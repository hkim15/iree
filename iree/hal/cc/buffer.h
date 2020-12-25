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

#ifndef IREE_HAL_CC_BUFFER_H_
#define IREE_HAL_CC_BUFFER_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "absl/types/span.h"
#include "iree/base/logging.h"
#include "iree/base/status.h"
#include "iree/hal/api.h"
#include "iree/hal/cc/resource.h"

namespace iree {
namespace hal {

class Allocator;

class Buffer : public Resource {
 public:
  // Disallow copies (as copying requires real work).
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  ~Buffer() override = default;

  absl::string_view debug_name() const { return ""; }
  void set_debug_name(std::string debug_name) {}

  // Memory allocator this buffer was allocated from.
  // May be nullptr if the buffer has no particular allocator and should be
  // assumed to be allocated from the host heap.
  constexpr Allocator* allocator() const {
    return allocated_buffer_ == this ? allocator_
                                     : allocated_buffer_->allocator();
  }

  // Memory type this buffer is allocated from.
  iree_hal_memory_type_t memory_type() const { return memory_type_; }

  // Memory access operations allowed on the buffer.
  iree_hal_memory_access_t allowed_access() const { return allowed_access_; }

  // Bitfield describing how the buffer is to be used.
  iree_hal_buffer_usage_t usage() const { return usage_; }

  Buffer* allocated_buffer() const noexcept {
    Buffer* allocated_buffer = allocated_buffer_;
    while (allocated_buffer != this &&
           allocated_buffer != allocated_buffer->allocated_buffer()) {
      allocated_buffer = allocated_buffer->allocated_buffer();
    }
    return allocated_buffer;
  }

  constexpr iree_device_size_t allocation_size() const {
    return allocated_buffer_ == this ? allocation_size_
                                     : allocated_buffer_->allocation_size();
  }

  constexpr iree_device_size_t byte_offset() const noexcept {
    return byte_offset_;
  }
  constexpr iree_device_size_t byte_length() const noexcept {
    return byte_length_;
  }

 protected:
  // Defines the mode of a MapMemory operation.
  enum class MappingMode {
    // The call to MapMemory will always be matched with UnmapMemory.
    kScoped,
  };

  Buffer(Allocator* allocator, iree_hal_memory_type_t memory_type,
         iree_hal_memory_access_t allowed_access, iree_hal_buffer_usage_t usage,
         iree_device_size_t allocation_size, iree_device_size_t byte_offset,
         iree_device_size_t byte_length)
      : allocated_buffer_(const_cast<Buffer*>(this)),
        allocator_(allocator),
        memory_type_(memory_type),
        allowed_access_(allowed_access),
        usage_(usage),
        allocation_size_(allocation_size),
        byte_offset_(byte_offset),
        byte_length_(byte_length) {}

  // Sets a range of the buffer to the given value.
  // State and parameters have already been validated. For the >8bit variants
  // the offset and length have already been validated to be aligned to the
  // natural alignment of the type.
  virtual Status FillImpl(iree_device_size_t byte_offset,
                          iree_device_size_t byte_length, const void* pattern,
                          iree_device_size_t pattern_length) = 0;

  // Reads a block of byte data from the resource at the given offset.
  // State and parameters have already been validated.
  virtual Status ReadDataImpl(iree_device_size_t source_offset, void* data,
                              iree_device_size_t data_length) = 0;

  // Writes a block of byte data into the resource at the given offset.
  // State and parameters have already been validated.
  virtual Status WriteDataImpl(iree_device_size_t target_offset,
                               const void* data,
                               iree_device_size_t data_length) = 0;

  // Copies a block of byte data into the resource at the given offset.
  // State and parameters have already been validated.
  virtual Status CopyDataImpl(iree_device_size_t target_offset,
                              Buffer* source_buffer,
                              iree_device_size_t source_offset,
                              iree_device_size_t data_length) = 0;

  // Maps memory directly.
  // The output data pointer will be properly aligned to the start of the data.
  // |local_byte_offset| and |local_byte_length| are the adjusted values that
  // should map into the local space of the buffer.
  //
  // Fails if the memory could not be mapped (invalid access type, invalid
  // range, or unsupported memory type).
  // State and parameters have already been validated.
  virtual Status MapMemoryImpl(MappingMode mapping_mode,
                               iree_hal_memory_access_t memory_access,
                               iree_device_size_t local_byte_offset,
                               iree_device_size_t local_byte_length,
                               void** out_data) = 0;

  // Unmaps previously mapped memory.
  // No-op if the memory is not mapped. As this is often used in destructors
  // we can't rely on failures here propagating with anything but
  // IREE_CHECK/IREE_DCHECK. State and parameters have already been validated.
  virtual Status UnmapMemoryImpl(iree_device_size_t local_byte_offset,
                                 iree_device_size_t local_byte_length,
                                 void* data) = 0;

  // Invalidates ranges of non-coherent memory from the host caches.
  // Use this before reading from non-coherent memory.
  // This guarantees that device writes to the memory ranges provided are
  // visible on the host.
  // This is only required for memory types without kHostCoherent set.
  // State and parameters have already been validated.
  virtual Status InvalidateMappedMemoryImpl(
      iree_device_size_t local_byte_offset,
      iree_device_size_t local_byte_length) = 0;

  // Flushes ranges of non-coherent memory from the host caches.
  // Use this after writing to non-coherent memory.
  // This guarantees that host writes to the memory ranges provided are made
  // available for device access.
  // This is only required for memory types without kHostCoherent set.
  // State and parameters have already been validated.
  virtual Status FlushMappedMemoryImpl(
      iree_device_size_t local_byte_offset,
      iree_device_size_t local_byte_length) = 0;

 private:
  friend class Allocator;

  // Points to either this or parent_buffer_.get().
  Buffer* allocated_buffer_ = nullptr;

  Allocator* allocator_ = nullptr;
  iree_hal_memory_type_t memory_type_ = IREE_HAL_MEMORY_TYPE_NONE;
  iree_hal_memory_access_t allowed_access_ = IREE_HAL_MEMORY_ACCESS_NONE;
  iree_hal_buffer_usage_t usage_ = IREE_HAL_BUFFER_USAGE_NONE;

  iree_device_size_t allocation_size_ = 0;
  iree_device_size_t byte_offset_ = 0;
  iree_device_size_t byte_length_ = 0;

  // Defined when this buffer is a subspan of another buffer.
  ref_ptr<Buffer> parent_buffer_;
};

}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_CC_BUFFER_H_
