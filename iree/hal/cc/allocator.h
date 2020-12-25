// Copyright 2019 Google LLC
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

#ifndef IREE_HAL_CC_ALLOCATOR_H_
#define IREE_HAL_CC_ALLOCATOR_H_

#include <cstddef>
#include <memory>

#include "absl/types/span.h"
#include "iree/base/ref_ptr.h"
#include "iree/base/status.h"
#include "iree/hal/cc/buffer.h"

namespace iree {
namespace hal {

class Allocator : public RefObject<Allocator> {
 public:
  virtual ~Allocator() = default;

  virtual bool CanUseBufferLike(
      Allocator* source_allocator, iree_hal_memory_type_t memory_type,
      iree_hal_buffer_usage_t buffer_usage,
      iree_hal_buffer_usage_t intended_usage) const = 0;

  virtual StatusOr<ref_ptr<Buffer>> Allocate(
      iree_hal_memory_type_t memory_type, iree_hal_buffer_usage_t buffer_usage,
      size_t allocation_size) = 0;
};

}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_CC_ALLOCATOR_H_
