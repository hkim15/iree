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

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

cc_library(
    name = "init_conversions",
    hdrs = [
        "init_conversions.h",
    ],
    deps = [
        "//iree/compiler/Conversion/HLOToLinalg",
        "//iree/compiler/Conversion/LinalgToLLVM",
        "//iree/compiler/Conversion/LinalgToSPIRV",
        "//iree/compiler/Conversion/LinalgToVector",
    ],
)
