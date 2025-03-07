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

#ifndef IREE_HAL_VMLA_VMLA_CACHE_H_
#define IREE_HAL_VMLA_VMLA_CACHE_H_

#include "iree/hal/allocator.h"
#include "iree/hal/executable.h"
#include "iree/hal/executable_cache.h"
#include "iree/vm/api.h"

namespace iree {
namespace hal {
namespace vmla {

class VMLACache final : public ExecutableCache {
 public:
  explicit VMLACache(iree_vm_instance_t* instance,
                     iree_vm_module_t* vmla_module);
  ~VMLACache() override;

  bool CanPrepareFormat(ExecutableFormat format) const override;

  StatusOr<ref_ptr<Executable>> PrepareExecutable(
      ExecutableLayout* executable_layout, ExecutableCachingModeBitfield mode,
      const ExecutableSpec& spec) override;

 private:
  iree_vm_instance_t* instance_ = nullptr;
  iree_vm_module_t* vmla_module_ = nullptr;
};

}  // namespace vmla
}  // namespace hal
}  // namespace iree

#endif  // IREE_HAL_VMLA_VMLA_CACHE_H_
