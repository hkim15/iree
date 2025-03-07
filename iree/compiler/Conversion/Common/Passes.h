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

namespace mlir {
namespace iree_compiler {

/// Pass to legalize function that returns number of workgroups to use for
/// launch to be runnable on the host.
std::unique_ptr<OperationPass<ModuleOp>> createLegalizeNumWorkgroupsFnPass();

/// Pass to initialize the function that computes the number of workgroups for
/// each entry point function. The function is defined, but is populated later.
std::unique_ptr<OperationPass<ModuleOp>> createDeclareNumWorkgroupsFnPass();

/// Pass to optimize vector transfer_read and transfer_write.
std::unique_ptr<FunctionPass> createVectorTransferOptimizationPass();

}  // namespace iree_compiler
}  // namespace mlir
