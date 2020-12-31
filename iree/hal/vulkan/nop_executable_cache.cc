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

#include "iree/hal/vulkan/nop_executable_cache.h"

#include "iree/base/tracing.h"
#include "iree/hal/vulkan/native_executable.h"

using namespace iree::hal::vulkan;

static const iree_hal_executable_format_t kExecutableFormatSpirV =
    iree_hal_make_executable_format("SPVE");

typedef struct {
  iree_hal_resource_t resource;
  VkDeviceHandle* logical_device;
} iree_hal_vulkan_nop_executable_cache_t;

static const iree_hal_executable_cache_vtable_t
    iree_hal_vulkan_nop_executable_cache_vtable;

iree_status_t iree_hal_vulkan_nop_executable_cache_create(
    iree::hal::vulkan::VkDeviceHandle* logical_device,
    iree_string_view_t identifier,
    iree_hal_executable_cache_t** out_executable_cache) {
  IREE_ASSERT_ARGUMENT(out_executable_cache);
  *out_executable_cache = NULL;
  IREE_TRACE_ZONE_BEGIN(z0);

  iree_hal_vulkan_nop_executable_cache_t* executable_cache = NULL;
  iree_status_t status = iree_allocator_malloc(logical_device->host_allocator(),
                                               sizeof(*executable_cache),
                                               (void**)&executable_cache);
  if (iree_status_is_ok(status)) {
    iree_hal_resource_initialize(&iree_hal_vulkan_nop_executable_cache_vtable,
                                 &executable_cache->resource);
    executable_cache->logical_device = logical_device;

    *out_executable_cache = (iree_hal_executable_cache_t*)executable_cache;
  }

  IREE_TRACE_ZONE_END(z0);
  return status;
}

static void iree_hal_vulkan_nop_executable_cache_destroy(
    iree_hal_executable_cache_t* base_executable_cache) {
  iree_hal_vulkan_nop_executable_cache_t* executable_cache =
      (iree_hal_vulkan_nop_executable_cache_t*)base_executable_cache;
  iree_allocator_t host_allocator =
      executable_cache->logical_device->host_allocator();
  IREE_TRACE_ZONE_BEGIN(z0);

  iree_allocator_free(host_allocator, executable_cache);

  IREE_TRACE_ZONE_END(z0);
}

static bool iree_hal_vulkan_nop_executable_cache_can_prepare_format(
    iree_hal_executable_cache_t* base_executable_cache,
    iree_hal_executable_format_t format) {
  return format == kExecutableFormatSpirV;
}

static iree_status_t iree_hal_vulkan_nop_executable_cache_prepare_executable(
    iree_hal_executable_cache_t* base_executable_cache,
    iree_hal_executable_layout_t* executable_layout,
    iree_hal_executable_caching_mode_t caching_mode,
    iree_const_byte_span_t executable_data,
    iree_hal_executable_t** out_executable) {
  iree_hal_vulkan_nop_executable_cache_t* executable_cache =
      (iree_hal_vulkan_nop_executable_cache_t*)base_executable_cache;
  return iree_hal_vulkan_native_executable_create(
      executable_cache->logical_device,
      /*pipeline_cache=*/VK_NULL_HANDLE, executable_layout, caching_mode,
      executable_data, out_executable);
}

static const iree_hal_executable_cache_vtable_t
    iree_hal_vulkan_nop_executable_cache_vtable = {
        /* .destroy = */ iree_hal_vulkan_nop_executable_cache_destroy,
        /* .can_prepare_format = */
        iree_hal_vulkan_nop_executable_cache_can_prepare_format,
        /* .prepare_executable = */
        iree_hal_vulkan_nop_executable_cache_prepare_executable,
};
