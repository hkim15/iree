// RUN: iree-opt -split-input-file -iree-vmla-conversion %s | IreeFileCheck %s

// CHECK-LABEL: func private @basic
func private @basic(%arg0 : tensor<5xf32>) -> (tensor<5xi32>) {
  // CHECK: vmla.convert
  %0 = "mhlo.convert"(%arg0) : (tensor<5xf32>) -> tensor<5xi32>
  return %0 : tensor<5xi32>
}

// CHECK-LABEL: func private @noop
func private @noop(%arg0 : tensor<?xf32>) -> (tensor<5xf32>) {
  // CHECK: return %arg0
  %0 = "mhlo.convert"(%arg0) : (tensor<?xf32>) -> tensor<5xf32>
  return %0 : tensor<5xf32>
}
