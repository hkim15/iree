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

package(
    default_visibility = ["//visibility:public"],
    features = ["layering_check"],
    licenses = ["notice"],  # Apache 2.0
)

cc_library(
    name = "CodeGenOptionUtils",
    srcs = [
        "CodeGenOptionUtils.cpp",
    ],
    hdrs = [
        "CodeGenOptionUtils.h",
    ],
    deps = [
        "@llvm-project//llvm:Support",
    ],
)

cc_library(
    name = "LinalgToSPIRV",
    srcs = [
        "ConvertToGPUPass.cpp",
        "ConvertToSPIRVPass.cpp",
        "CooperativeMatrixAnalysis.cpp",
        "FoldGPUProcessorIDUses.cpp",
        "KernelDispatchUtils.cpp",
        "LinalgTileAndFusePass.cpp",
        "MatMulVectorizationTest.cpp",
        "Passes.cpp",
        "SplitDispatchFunctionPass.cpp",
        "Utils.cpp",
        "VectorToGPUPass.cpp",
        "VectorizeMemref.cpp",
    ],
    hdrs = [
        "CooperativeMatrixAnalysis.h",
        "KernelDispatchUtils.h",
        "MemorySpace.h",
        "Passes.h",
        "Utils.h",
    ],
    deps = [
        ":CodeGenOptionUtils",
        "//iree/compiler/Conversion/CodegenUtils",
        "//iree/compiler/Conversion/Common",
        "//iree/compiler/Conversion/HLOToHLO",
        "//iree/compiler/Conversion/HLOToLinalg",
        "//iree/compiler/Conversion/LinalgToVector",
        "//iree/compiler/Dialect/HAL/IR",
        "//iree/compiler/Dialect/IREE/IR",
        "//iree/compiler/Dialect/Shape/IR",
        "//iree/compiler/Dialect/Shape/Transforms",
        "@llvm-project//llvm:Support",
        "@llvm-project//mlir:Affine",
        "@llvm-project//mlir:AffineToStandardTransforms",
        "@llvm-project//mlir:Analysis",
        "@llvm-project//mlir:GPUDialect",
        "@llvm-project//mlir:GPUToSPIRV",
        "@llvm-project//mlir:GPUTransforms",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:LinalgOps",
        "@llvm-project//mlir:LinalgTransforms",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:SCFDialect",
        "@llvm-project//mlir:SCFToGPUPass",
        "@llvm-project//mlir:SCFToSPIRV",
        "@llvm-project//mlir:SPIRVConversion",
        "@llvm-project//mlir:SPIRVDialect",
        "@llvm-project//mlir:SPIRVTransforms",
        "@llvm-project//mlir:SideEffectInterfaces",
        "@llvm-project//mlir:StandardOps",
        "@llvm-project//mlir:StandardToSPIRV",
        "@llvm-project//mlir:Support",
        "@llvm-project//mlir:Transforms",
        "@llvm-project//mlir:VectorOps",
        "@llvm-project//mlir:VectorToSPIRV",
        "@org_tensorflow//tensorflow/compiler/mlir/hlo",
        "@org_tensorflow//tensorflow/compiler/mlir/hlo:legalize_to_linalg",
    ],
)
