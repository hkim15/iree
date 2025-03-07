// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IREE_INTEGRATIONS_TFSTRINGS_CONVERSION_CONVERT_TF_STRINGS_TO_STRINGS_H_
#define IREE_INTEGRATIONS_TFSTRINGS_CONVERSION_CONVERT_TF_STRINGS_TO_STRINGS_H_

#include "integrations/tensorflow/compiler/dialect/tf_strings/ir/types.h"
#include "iree/compiler/Dialect/Modules/Strings/IR/Dialect.h"
#include "iree/compiler/Dialect/Modules/Strings/IR/Types.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Pass/Pass.h"

namespace mlir {
namespace iree_integrations {
namespace tf_strings {

std::unique_ptr<OperationPass<ModuleOp>> createConvertTFStringsToStringsPass();

// Populates conversion patterns from the tensor-based custom dialect ops to the
// HAL buffer-based ones.
void populateTFStringsToStringsPatterns(MLIRContext *context,
                                        OwningRewritePatternList &patterns);

}  // namespace tf_strings
}  // namespace iree_integrations
}  // namespace mlir

#endif  // IREE_INTEGRATIONS_TFSTRINGS_TRANSFORMS_TFSTRINGSTOSTRINGS_H_
