# Copyright 2019 Google LLC
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

# Conformance Test Suite (CTS) for HAL implementations.

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

cc_library(
    name = "cts_test_base",
    testonly = True,
    hdrs = ["cts_test_base.h"],
    data = [
        # For AddressSanitizer when using Vulkan + a local Nvidia GPU
        "//iree/tools:sanitizer_suppressions.txt",
    ],
    deps = [
        "//iree/base:status",
        "//iree/hal",
        "//iree/hal:api",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
    ],
)

cc_test(
    name = "allocator_test",
    srcs = ["allocator_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/base:status",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)

cc_test(
    name = "buffer_test",
    srcs = ["buffer_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/base:status",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)

cc_test(
    name = "command_buffer_test",
    srcs = ["command_buffer_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/base:status",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)

cc_test(
    name = "command_queue_test",
    srcs = ["command_queue_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/base:status",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)

cc_test(
    name = "driver_test",
    srcs = ["driver_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)

cc_test(
    name = "semaphore_test",
    srcs = ["semaphore_test.cc"],
    deps = [
        ":cts_test_base",
        "//iree/hal/testing:driver_registry",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
    ],
)
