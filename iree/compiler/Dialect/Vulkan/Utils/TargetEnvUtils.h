// Copyright 2020 Google LLC
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

#ifndef IREE_COMPILER_DIALECT_VULKAN_UTILS_TARGETENVUTILS_H_
#define IREE_COMPILER_DIALECT_VULKAN_UTILS_TARGETENVUTILS_H_

#include "iree/compiler/Dialect/Vulkan/IR/VulkanAttributes.h"
#include "mlir/Dialect/SPIRV/IR/TargetAndABI.h"

namespace mlir {
namespace iree_compiler {
namespace IREE {
namespace Vulkan {

/// Returns the Vulkan target environment assembly for the given GPU triple.
/// Returns nullptr if the triple is not recognized.
///
/// We call it "triple" to match common compiler language: historically one
/// would describe a CPU compiler target as a string containing exactly three
/// fields. But here the configuration is for GPU and there can exist a lot of
/// architectures/vendors/SoCs/OSes. We define it in the form of:
///   <arch/vendor>-<gpu>-<soc/product>-<os>
/// For example:
///   ampere-rtx3080-unknown-windows
///   rdna-5700xt-unkown-linux
///   qualcomm-adreno640-pixel4-android10
///   valhall-g77-galaxys20-android10
/// "unknown" means the parameter is not important.
const char* getTargetEnvForTriple(llvm::StringRef triple);

/// Converts the given Vulkan target environment into the corresponding SPIR-V
/// target environment.
///
/// Vulkan and SPIR-V are two different domains working closely. A Vulkan target
/// environment specifies the Vulkan version, extensions, features, and resource
/// limits queried from a Vulkan implementation. These properties typically have
/// corresponding SPIR-V bits, directly or indirectly. For example, by default,
/// Vulkan 1.0 supports SPIR-V 1.0 and Vulkan 1.1 supports up to SPIR-V 1.3.
/// If the VK_KHR_spirv_1_4 extension is available, then SPIR-V 1.4 can be used.
/// Similarly, if the VK_KHR_variable_pointers extension is available, then
/// the VariablePointersStorageBuffer capabilities on SPIR-V side can be
/// activated. The function handles the mapping relationship between tese two
/// domains.
spirv::TargetEnvAttr convertTargetEnv(Vulkan::TargetEnvAttr vkTargetEnv);

}  // namespace Vulkan
}  // namespace IREE
}  // namespace iree_compiler
}  // namespace mlir

#endif  // IREE_COMPILER_DIALECT_VULKAN_UTILS_TARGETENVUTILS_H_
