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

#ifndef IREE_HAL_CC_COMMAND_BUFFER_H_
#define IREE_HAL_CC_COMMAND_BUFFER_H_

#include <cstdint>

#include "iree/base/status.h"
#include "iree/hal/cc/buffer.h"
#include "iree/hal/cc/resource.h"

namespace iree {
namespace hal {

class CommandBuffer : public Resource {
 public:
  iree_hal_command_buffer_mode_t mode() const { return mode_; }

  virtual Status Begin() = 0;

  virtual Status End() = 0;

  virtual Status ExecutionBarrier(
      iree_hal_execution_stage_t source_stage_mask,
      iree_hal_execution_stage_t target_stage_mask,
      absl::Span<const iree_hal_memory_barrier_t> memory_barriers,
      absl::Span<const iree_hal_buffer_barrier_t> buffer_barriers) = 0;

  virtual Status SignalEvent(iree_hal_event_t* event,
                             iree_hal_execution_stage_t source_stage_mask) = 0;

  virtual Status ResetEvent(iree_hal_event_t* event,
                            iree_hal_execution_stage_t source_stage_mask) = 0;

  virtual Status WaitEvents(
      absl::Span<iree_hal_event_t*> events,
      iree_hal_execution_stage_t source_stage_mask,
      iree_hal_execution_stage_t target_stage_mask,
      absl::Span<const iree_hal_memory_barrier_t> memory_barriers,
      absl::Span<const iree_hal_buffer_barrier_t> buffer_barriers) = 0;

  virtual Status FillBuffer(Buffer* target_buffer,
                            iree_device_size_t target_offset,
                            iree_device_size_t length, const void* pattern,
                            size_t pattern_length) = 0;

  virtual Status DiscardBuffer(Buffer* buffer) = 0;

  virtual Status UpdateBuffer(const void* source_buffer,
                              iree_device_size_t source_offset,
                              Buffer* target_buffer,
                              iree_device_size_t target_offset,
                              iree_device_size_t length) = 0;

  virtual Status CopyBuffer(Buffer* source_buffer,
                            iree_device_size_t source_offset,
                            Buffer* target_buffer,
                            iree_device_size_t target_offset,
                            iree_device_size_t length) = 0;

  virtual Status PushConstants(iree_hal_executable_layout_t* executable_layout,
                               size_t offset,
                               absl::Span<const uint32_t> values) = 0;

  virtual Status PushDescriptorSet(
      iree_hal_executable_layout_t* executable_layout, int32_t set,
      absl::Span<const iree_hal_descriptor_set_binding_t> bindings) = 0;

  virtual Status BindDescriptorSet(
      iree_hal_executable_layout_t* executable_layout, int32_t set,
      iree_hal_descriptor_set_t* descriptor_set,
      absl::Span<const iree_device_size_t> dynamic_offsets) = 0;

  virtual Status Dispatch(iree_hal_executable_t* executable,
                          int32_t entry_point,
                          std::array<uint32_t, 3> workgroups) = 0;

  virtual Status DispatchIndirect(iree_hal_executable_t* executable,
                                  int32_t entry_point,
                                  Buffer* workgroups_buffer,
                                  iree_device_size_t workgroups_offset) = 0;

 protected:
  CommandBuffer(iree_hal_command_buffer_mode_t mode,
                iree_hal_command_category_t command_categories)
      : mode_(mode) {}

 private:
  const iree_hal_command_buffer_mode_t mode_;
};

}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_CC_COMMAND_BUFFER_H_
