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

#ifndef IREE_COMPILER_DIALECT_HAL_CONVERSION_CONVERTIREETOHAL_H_
#define IREE_COMPILER_DIALECT_HAL_CONVERSION_CONVERTIREETOHAL_H_

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Transforms/DialectConversion.h"

namespace mlir {
namespace iree_compiler {

// TODO(gcmn): Use conversion interfaces. Requires breaking circular dependency
// between HAL and IREE dialects.

// Appends all patterns for lowering IREE ops to HAL buffer ops.
void populateIREEToHALPatterns(MLIRContext *context,
                               OwningRewritePatternList &patterns);

// Setup the |conversionTarget| op legality to ensure helpful error messages for
// IREE ops we know should always be converted.
void setupIREEToHALLegality(MLIRContext *context, ConversionTarget &target);

}  // namespace iree_compiler
}  // namespace mlir

#endif  // IREE_COMPILER_DIALECT_HAL_CONVERSION_CONVERTIREETOHAL_H_
