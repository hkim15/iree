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

#ifndef THIRD_PARTY_TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_TENSORLIST_TYPES_H_
#define THIRD_PARTY_TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_TENSORLIST_TYPES_H_

#include "iree/compiler/Dialect/IREE/IR/IREETypes.h"

namespace mlir {
namespace iree_integrations {
namespace tf_tensorlist {

class TensorListType
    : public Type::TypeBase<TensorListType, Type, TypeStorage> {
 public:
  using Base::Base;
};

}  // namespace tf_tensorlist
}  // namespace iree_integrations
}  // namespace mlir

#endif  // THIRD_PARTY_TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_TENSORLIST_TYPES_H_
