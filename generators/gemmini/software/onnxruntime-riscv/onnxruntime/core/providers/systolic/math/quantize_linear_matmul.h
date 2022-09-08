// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "core/common/common.h"
#include "core/framework/op_kernel.h"
#include "core/mlas/inc/mlas.h"

namespace onnxruntime {
namespace systolic {

template <typename T1, typename T2, typename T3>
class QLinearMatMul final : public OpKernel {
 public:
  QLinearMatMul(const OpKernelInfo& info) : OpKernel(info) {
  }

  Status Compute(OpKernelContext* context) const override;

};

} // namespace systolic
}  // namespace onnxruntime
