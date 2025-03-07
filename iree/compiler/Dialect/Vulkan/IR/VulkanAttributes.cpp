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

#include "iree/compiler/Dialect/Vulkan/IR/VulkanAttributes.h"

#include "iree/compiler/Dialect/Vulkan/IR/VulkanTypes.h"
#include "mlir/IR/AttributeSupport.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Diagnostics.h"
#include "mlir/IR/Identifier.h"
#include "mlir/IR/Location.h"

namespace mlir {
namespace iree_compiler {
namespace IREE {

#include "iree/compiler/Dialect/Vulkan/IR/VulkanAttributes.cpp.inc"

namespace Vulkan {

namespace detail {
struct TargetEnvAttributeStorage : public AttributeStorage {
  using KeyTy = std::tuple<Attribute, Attribute, Attribute, spirv::Vendor,
                           spirv::DeviceType, uint32_t, Attribute>;

  TargetEnvAttributeStorage(Attribute version, Attribute revision,
                            Attribute extensions, spirv::Vendor vendorID,
                            spirv::DeviceType deviceType, uint32_t deviceID,
                            Attribute capabilities)
      : version(version),
        revision(revision),
        extensions(extensions),
        capabilities(capabilities),
        vendorID(vendorID),
        deviceType(deviceType),
        deviceID(deviceID) {}

  bool operator==(const KeyTy &key) const {
    return key == std::make_tuple(version, revision, extensions, vendorID,
                                  deviceType, deviceID, capabilities);
  }

  static TargetEnvAttributeStorage *construct(
      AttributeStorageAllocator &allocator, const KeyTy &key) {
    return new (allocator.allocate<TargetEnvAttributeStorage>())
        TargetEnvAttributeStorage(std::get<0>(key), std::get<1>(key),
                                  std::get<2>(key), std::get<3>(key),
                                  std::get<4>(key), std::get<5>(key),
                                  std::get<6>(key));
  }

  Attribute version;
  Attribute revision;
  Attribute extensions;
  Attribute capabilities;
  spirv::Vendor vendorID;
  spirv::DeviceType deviceType;
  uint32_t deviceID;
};
}  // namespace detail

TargetEnvAttr TargetEnvAttr::get(Vulkan::Version version, uint32_t revision,
                                 ArrayRef<Vulkan::Extension> extensions,
                                 spirv::Vendor vendorID,
                                 spirv::DeviceType deviceType,
                                 uint32_t deviceID,
                                 DictionaryAttr capabilities) {
  mlir::Builder builder(capabilities.getContext());
  llvm::SmallVector<Attribute, 0> extAttrs;
  extAttrs.reserve(extensions.size());
  for (auto ext : extensions) {
    extAttrs.push_back(builder.getStringAttr(Vulkan::stringifyExtension(ext)));
  }
  return get(builder.getI32IntegerAttr(static_cast<uint32_t>(version)),
             builder.getI32IntegerAttr(revision),
             builder.getArrayAttr(extAttrs), vendorID, deviceType, deviceID,
             capabilities);
}

TargetEnvAttr TargetEnvAttr::get(IntegerAttr version, IntegerAttr revision,
                                 ArrayAttr extensions, spirv::Vendor vendorID,
                                 spirv::DeviceType deviceType,
                                 uint32_t deviceID,
                                 DictionaryAttr capabilities) {
  assert(version && revision && extensions && capabilities);
  MLIRContext *context = version.getContext();
  return Base::get(context, version, revision, extensions, vendorID, deviceType,
                   deviceID, capabilities);
}

StringRef TargetEnvAttr::getKindName() { return "target_env"; }

Version TargetEnvAttr::getVersion() {
  return static_cast<Version>(
      getImpl()->version.cast<IntegerAttr>().getValue().getZExtValue());
}

unsigned TargetEnvAttr::getRevision() {
  return getImpl()->revision.cast<IntegerAttr>().getValue().getZExtValue();
}

TargetEnvAttr::ext_iterator::ext_iterator(ArrayAttr::iterator it)
    : llvm::mapped_iterator<ArrayAttr::iterator, Extension (*)(Attribute)>(
          it, [](Attribute attr) {
            return *symbolizeExtension(attr.cast<StringAttr>().getValue());
          }) {}

TargetEnvAttr::ext_range TargetEnvAttr::getExtensions() {
  auto range = getExtensionsAttr().getValue();
  return {ext_iterator(range.begin()), ext_iterator(range.end())};
}

ArrayAttr TargetEnvAttr::getExtensionsAttr() {
  return getImpl()->extensions.cast<ArrayAttr>();
}

spirv::Vendor TargetEnvAttr::getVendorID() { return getImpl()->vendorID; }

spirv::DeviceType TargetEnvAttr::getDeviceType() {
  return getImpl()->deviceType;
}

uint32_t TargetEnvAttr::getDeviceID() { return getImpl()->deviceID; }

CapabilitiesAttr TargetEnvAttr::getCapabilitiesAttr() {
  return getImpl()->capabilities.cast<CapabilitiesAttr>();
}

LogicalResult TargetEnvAttr::verifyConstructionInvariants(
    Location loc, IntegerAttr version, IntegerAttr revision,
    ArrayAttr extensions, spirv::Vendor /*vendorID*/,
    spirv::DeviceType /*deviceType*/, uint32_t /*deviceID*/,
    DictionaryAttr capabilities) {
  if (!version.getType().isInteger(32))
    return emitError(loc) << "expected 32-bit integer for version";

  if (!revision.getType().isInteger(32))
    return emitError(loc) << "expected 32-bit integer for revision";

  if (!llvm::all_of(extensions.getValue(), [](Attribute attr) {
        if (auto strAttr = attr.dyn_cast<StringAttr>())
          if (symbolizeExtension(strAttr.getValue())) return true;
        return false;
      }))
    return emitError(loc) << "unknown extension in extension list";

  if (!capabilities.isa<CapabilitiesAttr>()) {
    return emitError(loc)
           << "expected vulkan::CapabilitiesAttr for capabilities";
  }

  return success();
}

}  // namespace Vulkan
}  // namespace IREE
}  // namespace iree_compiler
}  // namespace mlir
