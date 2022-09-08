// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/providers/systolic/systolic_execution_provider.h"
#include "core/framework/op_kernel.h"
#include "core/framework/kernel_registry.h"
#include "systolic_fwd.h"
#include "core/framework/compute_capability.h"

#ifdef ENABLE_TRAINING
#include "orttraining/training_ops/systolic/systolic_training_kernels.h"
#endif

#ifndef DISABLE_CONTRIB_OPS
#include "contrib_ops/systolic/systolic_contrib_kernels.h"
#endif

#include "fusion/fusion_ops.h"

namespace onnxruntime {

namespace systolic {

// Forward declarations of op kernels

#ifdef SYSTOLIC_INT8
class ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearMatMul);
class ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearConv);
class ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, int8_t, QLinearConvTranspose);
class ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearConv_nhwc);
#endif

#ifdef SYSTOLIC_FP32
class ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 8, float, MatMul);
class ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 9, float, MatMul);
// I have no idea why ORT originally declared Conv v1-10 and 11 as two separate things.
class ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 10, Conv);
class ONNX_OPERATOR_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 11, Conv);
class ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, Conv_nhwc);
class ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, MaxPool_nhwc);
class ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 7, 13, float, Gemm);
#endif

static Status RegisterSystolicKernels(KernelRegistry& kernel_registry) {
    static const BuildKernelCreateInfoFn function_table[] = {
#ifdef SYSTOLIC_INT8
      BuildKernelCreateInfo<ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearMatMul)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearConv)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, int8_t, QLinearConvTranspose)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 10, int8_t, QLinearConv_nhwc)>, 
#endif
#ifdef SYSTOLIC_FP32
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 8, float, MatMul)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 9, float, MatMul)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 10, Conv)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 11, Conv)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, Conv_nhwc)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 1, 11, MaxPool_nhwc)>,
      BuildKernelCreateInfo<ONNX_OPERATOR_VERSIONED_TYPED_KERNEL_CLASS_NAME(kSystolicExecutionProvider, kOnnxDomain, 7, 13, float, Gemm)>,
#endif
     };

  for (auto& function_table_entry : function_table) {
    ORT_RETURN_IF_ERROR(kernel_registry.Register(function_table_entry()));
  }

  #ifndef DISABLE_CONTRIB_OPS
    ORT_RETURN_IF_ERROR(::onnxruntime::systolic::RegisterSystolicContribKernels(kernel_registry));
  #endif
  #ifdef ENABLE_TRAINING
    ORT_RETURN_IF_ERROR(::onnxruntime::systolic::RegisterSystolicTrainingKernels(kernel_registry));
  #endif


  return Status::OK();
}

struct KernelRegistryAndStatus {
  std::shared_ptr<KernelRegistry> kernel_registry = std::make_shared<KernelRegistry>();
  Status st;
};

KernelRegistryAndStatus GetSystolicKernelRegistry() {
  KernelRegistryAndStatus ret;
  ret.st = RegisterSystolicKernels(*ret.kernel_registry);
  return ret;
}

}  // namespace systolic


void SystolicExecutionProvider::InsertFusedRules(FuseRuleFn rule) {
  fuse_rules_.push_back(rule);
}

std::shared_ptr<KernelRegistry> SystolicExecutionProvider::GetKernelRegistry() const {
  static systolic::KernelRegistryAndStatus k = onnxruntime::systolic::GetSystolicKernelRegistry();
  //throw if the registry failed to initialize
  ORT_THROW_IF_ERROR(k.st);
  return k.kernel_registry;
} 

std::unique_ptr<IDataTransfer> SystolicExecutionProvider::GetDataTransfer() const {
  return std::make_unique<CPUDataTransfer>();
}

char SystolicExecutionProvider::GetAcceleratorMode() const {
  return provider_info_.accelerator_mode;
}

std::vector<std::unique_ptr<ComputeCapability>>
SystolicExecutionProvider::GetCapability(const onnxruntime::GraphViewer& graph,
                                    const std::vector<const KernelRegistry*>& kernel_registries) const {
  std::vector<std::unique_ptr<ComputeCapability>>
      result = IExecutionProvider::GetCapability(graph, kernel_registries);

  for (auto& rule : fuse_rules_) {
    rule(graph, result);
  }
  return result;
}

void SystolicExecutionProvider::SetupFusedRules() {
  InsertFusedRules(onnxruntime::systolic::qlinearconv_relu_fuse());
#ifndef DISABLE_CONTRIB_OPS
  InsertFusedRules(onnxruntime::systolic::qlinearadd_relu_fuse());
#endif
}

}  // namespace onnxruntime
