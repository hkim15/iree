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

#ifndef IREE_HAL_CC_DEVICE_H_
#define IREE_HAL_CC_DEVICE_H_

#include <memory>

#include "iree/base/ref_ptr.h"
#include "iree/base/status.h"
#include "iree/base/target_platform.h"
#include "iree/base/time.h"
#include "iree/hal/cc/allocator.h"
#include "iree/hal/cc/buffer.h"
#include "iree/hal/cc/command_queue.h"
#include "iree/hal/cc/device_info.h"

#if defined(IREE_PLATFORM_WINDOWS)
// Win32 macro name conflicts:
#undef CreateEvent
#undef CreateSemaphore
#endif  // IREE_PLATFORM_WINDOWS

namespace iree {
namespace hal {

class Device : public RefObject<Device> {
 public:
  virtual ~Device() = default;

  const DeviceInfo& info() const { return device_info_; }

  virtual Allocator* allocator() const = 0;

  virtual Status CreateExecutableCache(
      iree_string_view_t identifier,
      iree_hal_executable_cache_t** out_executable_cache) = 0;

  virtual Status CreateDescriptorSetLayout(
      iree_hal_descriptor_set_layout_usage_type_t usage_type,
      absl::Span<const iree_hal_descriptor_set_layout_binding_t> bindings,
      iree_hal_descriptor_set_layout_t** out_descriptor_set_layout) = 0;

  virtual Status CreateExecutableLayout(
      absl::Span<iree_hal_descriptor_set_layout_t*> set_layouts,
      size_t push_constants,
      iree_hal_executable_layout_t** out_executable_layout) = 0;

  virtual Status CreateDescriptorSet(
      iree_hal_descriptor_set_layout_t* set_layout,
      absl::Span<const iree_hal_descriptor_set_binding_t> bindings,
      iree_hal_descriptor_set_t** out_descriptor_set) = 0;

  virtual StatusOr<ref_ptr<CommandBuffer>> CreateCommandBuffer(
      iree_hal_command_buffer_mode_t mode,
      iree_hal_command_category_t command_categories) = 0;

  virtual Status CreateEvent(iree_hal_event_t** out_event) = 0;

  virtual Status CreateSemaphore(uint64_t initial_value,
                                 iree_hal_semaphore_t** out_semaphore) = 0;

  virtual Status WaitAllSemaphores(
      const iree_hal_semaphore_list_t* semaphore_list,
      iree_time_t deadline_ns) = 0;

  virtual StatusOr<int> WaitAnySemaphore(
      const iree_hal_semaphore_list_t* semaphore_list,
      iree_time_t deadline_ns) = 0;

  virtual Status WaitIdle(iree_time_t deadline_ns) = 0;

 protected:
  explicit Device(DeviceInfo device_info)
      : device_info_(std::move(device_info)) {}

 private:
  const DeviceInfo device_info_;
};

}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_CC_DEVICE_H_
