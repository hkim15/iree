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

# Tests for end-to-end IREE support specific to the LLVM lowering.
# TODO(ravishankarm): Reorganize these tests.

load("//build_tools/bazel:iree_check_test.bzl", "iree_check_single_backend_test_suite")

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

iree_check_single_backend_test_suite(
    name = "check_llvm-aot-conv_img2col",
    srcs = [
        "conv.mlir",
    ],
    compiler_flags = [
        "-iree-codegen-linalg-to-llvm-conv-img2col-conversion=true",
    ],
    driver = "dylib",
    target_backend = "dylib-llvm-aot",
)

iree_check_single_backend_test_suite(
    name = "check_llvm-aot-exponential_fast",
    srcs = [
        "exponential.mlir",
    ],
    compiler_flags = [
        "-iree-codegen-linalg-to-llvm-fast-exp=true",
    ],
    driver = "dylib",
    target_backend = "dylib-llvm-aot",
)
