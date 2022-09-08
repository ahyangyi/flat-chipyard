// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <google/protobuf/stubs/status.h>

#include "environment.h"
#include "http_server.h"
#include "json_handling.h"
#include "executor.h"
#include "util.h"

namespace onnxruntime {
namespace server {

namespace protobufutil = google::protobuf::util;

#define GenerateErrorResponse(logger, error_code, message, context)                              \
  {                                                                                              \
    auto http_error_code = (error_code);                                                         \
    (context).response.insert(util::MS_REQUEST_ID_HEADER, ((context).request_id));               \
    if (!(context).client_request_id.empty()) {                                                  \
      (context).response.insert(util::MS_CLIENT_REQUEST_ID_HEADER, (context).client_request_id); \
    }                                                                                            \
    auto json_error_message = CreateJsonError(http_error_code, (message));                       \
    logger->debug(json_error_message);                                                           \
    (context).response.result(http_error_code);                                                  \
    (context).response.body() = json_error_message;                                              \
    (context).response.set(http::field::content_type, "application/json");                       \
  }

static bool ParseRequestPayload(const HttpContext& context, SupportedContentType request_type,
                                /* out */ PredictRequest& predictRequest, /* out */ http::status& error_code, /* out */ std::string& error_message);

void Predict(const std::string& name,
             const std::string& version,
             const std::string& action,
             /* in, out */ HttpContext& context,
             const std::shared_ptr<ServerEnvironment>& env) {
  auto logger = env->GetLogger(context.request_id);
  logger->info("Model Name: {}, Version: {}, Action: {}", name, version, action);

  auto effective_name = name.empty() ? "default" : name;
  auto effective_version = version.empty() ? "1" : version;

  if (!context.client_request_id.empty()) {
    logger->info("{}: [{}]", util::MS_CLIENT_REQUEST_ID_HEADER, context.client_request_id);
  }

  // Request and Response content type information
  SupportedContentType request_type = GetRequestContentType(context);
  SupportedContentType response_type = GetResponseContentType(context);
  if (response_type == SupportedContentType::Unknown) {
    GenerateErrorResponse(logger, http::status::bad_request, "Unknown 'Accept' header field in the request", context);
  }

  // Deserialize the payload
  auto body = context.request.body();
  PredictRequest predict_request{};
  http::status error_code;
  std::string error_message;
  bool parse_succeeded = ParseRequestPayload(context, request_type, predict_request, error_code, error_message);
  if (!parse_succeeded) {
    GenerateErrorResponse(logger, error_code, error_message, context);
    return;
  }

  // Run Prediction
  Executor executor(env.get(), context.request_id);
  PredictResponse predict_response{};
  auto status = executor.Predict(effective_name, effective_version, predict_request, predict_response);
  if (!status.ok()) {
    GenerateErrorResponse(logger, GetHttpStatusCode((status)), status.error_message(), context);
    return;
  }

  // Serialize to proper output format
  std::string response_body{};
  if (response_type == SupportedContentType::Json) {
    status = GenerateResponseInJson(predict_response, response_body);
    if (!status.ok()) {
      GenerateErrorResponse(logger, http::status::internal_server_error, status.error_message(), context);
      return;
    }
    context.response.set(http::field::content_type, "application/json");
  } else {
    response_body = predict_response.SerializeAsString();
    if (context.request.find("Accept") != context.request.end() && context.request["Accept"] != "*/*") {
      context.response.set(http::field::content_type, context.request["Accept"].to_string());
    } else {
      context.response.set(http::field::content_type, "application/octet-stream");
    }
  }

  // Build HTTP response
  context.response.insert(util::MS_REQUEST_ID_HEADER, context.request_id);
  if (!context.client_request_id.empty()) {
    context.response.insert(util::MS_CLIENT_REQUEST_ID_HEADER, context.client_request_id);
  }
  context.response.body() = response_body;
  context.response.result(http::status::ok);
};

static bool ParseRequestPayload(const HttpContext& context, SupportedContentType request_type, PredictRequest& predictRequest, http::status& error_code, std::string& error_message) {
  auto body = context.request.body();
  protobufutil::Status status;
  switch (request_type) {
    case SupportedContentType::Json: {
      status = GetRequestFromJson(body, predictRequest);
      if (!status.ok()) {
        error_code = GetHttpStatusCode(status);
        error_message = status.error_message();
        return false;
      }
      break;
    }
    case SupportedContentType::PbByteArray: {
      bool parse_succeeded = predictRequest.ParseFromArray(body.data(), static_cast<int>(body.size()));
      if (!parse_succeeded) {
        error_code = http::status::bad_request;
        error_message = "Invalid payload.";
        return false;
      }
      break;
    }
    default: {
      error_code = http::status::bad_request;
      error_message = "Missing or unknown 'Content-Type' header field in the request";
      return false;
    }
  }

  return true;
}

}  // namespace server
}  // namespace onnxruntime
