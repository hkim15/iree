# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

iree_add_all_subdirs()

iree_cc_library(
  NAME
    cc
  HDRS
    "allocator.h"
    "buffer.h"
    "command_buffer.h"
    "command_queue.h"
    "debug_capture_manager.h"
    "device.h"
    "device_info.h"
    "driver.h"
    "resource.h"
  DEPS
    absl::span
    absl::strings
    iree::base::core_headers
    iree::base::logging
    iree::base::ref_ptr
    iree::base::status
    iree::base::time
    iree::base::tracing
  PUBLIC
)
