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

#ifndef IREE_HAL_CC_COMMAND_QUEUE_H_
#define IREE_HAL_CC_COMMAND_QUEUE_H_

#include <cstdint>
#include <string>

#include "absl/types/span.h"
#include "iree/base/status.h"
#include "iree/base/time.h"
#include "iree/hal/cc/command_buffer.h"

namespace iree {
namespace hal {

class CommandQueue {
 public:
  virtual ~CommandQueue() = default;

  bool can_dispatch() const {
    return iree_all_bits_set(supported_categories_,
                             IREE_HAL_COMMAND_CATEGORY_DISPATCH);
  }

  virtual Status Submit(
      absl::Span<const iree_hal_submission_batch_t> batches) = 0;

  virtual Status WaitIdle(Time deadline_ns) = 0;
  inline Status WaitIdle(Duration timeout_ns) {
    return WaitIdle(RelativeTimeoutToDeadlineNanos(timeout_ns));
  }
  inline Status WaitIdle() { return WaitIdle(InfiniteFuture()); }

 protected:
  CommandQueue(std::string name,
               iree_hal_command_category_t supported_categories)
      : name_(std::move(name)), supported_categories_(supported_categories) {}

  const std::string name_;
  const iree_hal_command_category_t supported_categories_;
};

}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_CC_COMMAND_QUEUE_H_
