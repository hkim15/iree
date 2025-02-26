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

# A VMLA (VM-based Linear Algebra) runtime HAL backend.

load("//iree:build_defs.oss.bzl", "iree_cmake_extra_content")

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

iree_cmake_extra_content(
    content = """
if(NOT ${IREE_HAL_DRIVER_VMLA})
  return()
endif()
""",
)

cc_library(
    name = "op_kernels",
    hdrs = ["op_kernels.h"],
    textual_hdrs = [
        # TODO(benvanik): SIMD variants.
        "op_kernels_generic.h",
        "op_kernels_ruy.h",
        "op_kernels_fft.h",
    ],
    deps = [
        "//iree/base:status",
        "//iree/base:tracing",
        "@com_google_absl//absl/algorithm",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/types:span",
        "@com_google_ruy//ruy",
        "@com_google_ruy//ruy:context",
        "@pffft",
    ],
)

cc_test(
    name = "op_kernels_test",
    srcs = ["op_kernels_test.cc"],
    deps = [
        ":op_kernels",
        "//iree/base:core_headers",
        "//iree/testing:gtest",
        "//iree/testing:gtest_main",
        "@com_google_absl//absl/container:inlined_vector",
    ],
)

cc_library(
    name = "op_module",
    srcs = ["op_module.cc"],
    hdrs = ["op_module.h"],
    deps = [
        ":op_kernels",
        "//iree/base:api",
        "//iree/base:core_headers",
        "//iree/base:ref_ptr",
        "//iree/base:status",
        "//iree/base:tracing",
        "//iree/vm",
        "//iree/vm:cc",
        "@com_google_absl//absl/types:span",
    ],
)

cc_library(
    name = "vmla",
    srcs = [
        "vmla_cache.cc",
        "vmla_device.cc",
        "vmla_driver.cc",
        "vmla_executable.cc",
    ],
    hdrs = [
        "vmla_cache.h",
        "vmla_device.h",
        "vmla_driver.h",
        "vmla_executable.h",
    ],
    deps = [
        ":op_module",
        "//iree/base:api",
        "//iree/base:core_headers",
        "//iree/base:flatcc",
        "//iree/base:ref_ptr",
        "//iree/base:status",
        "//iree/base:tracing",
        "//iree/hal",
        "//iree/hal/host:host_buffer",
        "//iree/hal/host:host_executable",
        "//iree/hal/host:host_local_device",
        "//iree/hal/host/serial:serial_scheduling_model",
        "//iree/schemas:vmla_executable_def_c_fbs",
        "//iree/vm",
        "//iree/vm:bytecode_module",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
    ],
)
