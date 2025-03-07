// RUN: iree-opt -pass-pipeline='iree-hal-transformation-pipeline{serialize-executables=false}' -iree-hal-target-backends=vulkan-spirv %s | IreeFileCheck %s -check-prefix=DEFAULT
// RUN: iree-opt -pass-pipeline='iree-hal-transformation-pipeline{serialize-executables=false}' -iree-hal-target-backends=vulkan-spirv -iree-vulkan-target-triple=qualcomm-adreno640-unknown-android10 %s | IreeFileCheck %s -check-prefix=ADRENO640
// RUN: iree-opt -pass-pipeline='iree-hal-transformation-pipeline{serialize-executables=false}' -iree-hal-target-backends=vulkan-spirv -iree-vulkan-target-triple=valhall-g77-unknown-android10 %s | IreeFileCheck %s -check-prefix=MALIG77
// RUN: iree-opt -pass-pipeline='iree-hal-transformation-pipeline{serialize-executables=false}' -iree-hal-target-backends=vulkan-spirv -iree-vulkan-target-triple=turing-t4-unknown-linux %s | IreeFileCheck %s -check-prefix=TURINGT4

// TODO(antiagainst): Passing in lenghty strings as command-line options is not
// optimal. We should consider creating a dedicated test pass to pick up
// #vk.target_env in input assembly and convert them.

// DEFAULT: #spv.target_env<#spv.vce<v1.3, [Shader, GroupNonUniform, GroupNonUniformVote, GroupNonUniformArithmetic, GroupNonUniformBallot, GroupNonUniformShuffle, GroupNonUniformShuffleRelative], [SPV_KHR_storage_buffer_storage_class]>, SwiftShader:CPU, {cooperative_matrix_properties_nv = [], max_compute_shared_memory_size = 16384 : i32, max_compute_workgroup_invocations = 128 : i32, max_compute_workgroup_size = dense<[128, 128, 64]> : vector<3xi32>, subgroup_size = 4 : i32}>
// ADRENO640: #spv.target_env<#spv.vce<v1.3, [Shader, Int16, GroupNonUniform, GroupNonUniformVote, VariablePointers, VariablePointersStorageBuffer], [SPV_KHR_storage_buffer_storage_class, SPV_KHR_variable_pointers]>, Qualcomm:IntegratedGPU, {cooperative_matrix_properties_nv = [], max_compute_shared_memory_size = 32768 : i32, max_compute_workgroup_invocations = 1024 : i32, max_compute_workgroup_size = dense<[1024, 1024, 64]> : vector<3xi32>, subgroup_size = 64 : i32}>
// MALIG77: #spv.target_env<#spv.vce<v1.3, [Shader, Float16, Int16, Int8, StorageBuffer16BitAccess, StorageUniform16, StoragePushConstant16, StorageBuffer8BitAccess, UniformAndStorageBuffer8BitAccess, StoragePushConstant8, GroupNonUniform, VariablePointers, VariablePointersStorageBuffer], [SPV_KHR_16bit_storage, SPV_KHR_8bit_storage, SPV_KHR_storage_buffer_storage_class, SPV_KHR_variable_pointers]>, ARM:IntegratedGPU, {cooperative_matrix_properties_nv = [], max_compute_shared_memory_size = 32768 : i32, max_compute_workgroup_invocations = 512 : i32, max_compute_workgroup_size = dense<512> : vector<3xi32>, subgroup_size = 16 : i32}>
// TURINGT4: #spv.target_env<#spv.vce<v1.5, [Shader, Float64, Float16, Int64, Int16, Int8, StorageBuffer16BitAccess, StorageUniform16, StoragePushConstant16, StorageBuffer8BitAccess, UniformAndStorageBuffer8BitAccess, StoragePushConstant8, GroupNonUniform, GroupNonUniformVote, GroupNonUniformArithmetic, GroupNonUniformBallot, GroupNonUniformShuffle, GroupNonUniformShuffleRelative, VariablePointers, VariablePointersStorageBuffer, CooperativeMatrixNV], [SPV_KHR_16bit_storage, SPV_KHR_8bit_storage, SPV_KHR_storage_buffer_storage_class, SPV_KHR_variable_pointers, SPV_NV_cooperative_matrix]>, NVIDIA:DiscreteGPU, {cooperative_matrix_properties_nv = [{a_type = i8, b_type = i8, c_type = i32, k_size = 32 : i32, m_size = 8 : i32, n_size = 8 : i32, result_type = i32, scope = 3 : i32}, {a_type = f16, b_type = f16, c_type = f16, k_size = 16 : i32, m_size = 16 : i32, n_size = 16 : i32, result_type = f16, scope = 3 : i32}, {a_type = f16, b_type = f16, c_type = f32, k_size = 16 : i32, m_size = 16 : i32, n_size = 16 : i32, result_type = f32, scope = 3 : i32}], max_compute_shared_memory_size = 49152 : i32, max_compute_workgroup_invocations = 1024 : i32, max_compute_workgroup_size = dense<[2147483647, 65535, 65535]> : vector<3xi32>, subgroup_size = 32 : i32}>
flow.executable @simpleMath_ex_dispatch_0 {
  flow.dispatch.entry @simpleMath_rgn_dispatch_0 attributes {
      workload = 4 : index
  }
  module {
    func @simpleMath_rgn_dispatch_0(%arg0: tensor<4xf32>) -> tensor<4xf32> {
      %0 = mhlo.add %arg0, %arg0 : tensor<4xf32>
      return %0 : tensor<4xf32>
    }
  }
}
