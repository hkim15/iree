// RUN: iree-opt -allow-unregistered-dialect -split-input-file -iree-codegen-split-dispatch-function -verify-diagnostics %s | IreeFileCheck %s

module {
  //     CHECK: func @kernel_fusable_fill_conv_ops
  //     CHECK:   linalg.fill
  // CHECK-NOT:   return
  //     CHECK:   linalg.conv
  //     CHECK:   return

  func @kernel_fusable_fill_conv_ops()
  attributes {hal.num_workgroups_fn = @kernel_fusable_fill_conv_ops_num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %dim = hal.interface.load.constant offset = 0 : index
    %shape1 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,2,2,512]>
    %shape2 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,1,1,512]>
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x2x2x512xf32>
    %ts1 = shapex.tie_shape %0, %shape1 : memref<?x2x2x512xf32>, !shapex.ranked_shape<[?,2,2,512]>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
    %ts2 = shapex.tie_shape %2, %shape2 : memref<?x1x1x512xf32>, !shapex.ranked_shape<[?,1,1,512]>
    linalg.fill(%ts2, %cst) : memref<?x1x1x512xf32>, f32
    linalg.conv(%1, %ts1, %ts2) {dilations = [1, 1], padding = dense<[[0, 1], [0, 1]]> : tensor<2x2xi64>, strides = [2, 2]} : memref<3x3x512x1xf32>, memref<?x2x2x512xf32>, memref<?x1x1x512xf32>
    return
  }
  func @kernel_fusable_fill_conv_ops_num_workgroups__
    (!shapex.ranked_shape<[?,2,2,512]>, !shapex.ranked_shape<[3,3,512,1]>,
     !shapex.ranked_shape<[?,1,1,512]>) -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----

module {
  //     CHECK: func @kernel_fusable_fill_matmul_ops
  //     CHECK:   linalg.fill
  // CHECK-NOT:   return
  //     CHECK:   linalg.matmul
  //     CHECK:   return

  func @kernel_fusable_fill_matmul_ops()
  attributes {hal.num_workgroups_fn = @kernel_fusable_fill_matmul_ops_num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %dimM = hal.interface.load.constant offset = 0 : index
    %dimN = hal.interface.load.constant offset = 1 : index
    %shape1 = shapex.make_ranked_shape %dimM : (index) -> !shapex.ranked_shape<[?,512]>
    %shape2 = shapex.make_ranked_shape %dimN : (index) -> !shapex.ranked_shape<[512,?]>
    %shape3 = shapex.make_ranked_shape %dimM, %dimN : (index, index) -> !shapex.ranked_shape<[?,?]>
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x512xf32>
    %ts1 = shapex.tie_shape %0, %shape1 : memref<?x512xf32>, !shapex.ranked_shape<[?,512]>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<512x?xf32>
    %ts2 = shapex.tie_shape %1, %shape2 : memref<512x?xf32>, !shapex.ranked_shape<[512, ?]>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x?xf32>
    %ts3 = shapex.tie_shape %2, %shape3 : memref<?x?xf32>, !shapex.ranked_shape<[?,?]>
    linalg.fill(%ts3, %cst) : memref<?x?xf32>, f32
    linalg.matmul ins(%ts1, %ts2 : memref<?x512xf32>, memref<512x?xf32>)
                  outs(%ts3 : memref<?x?xf32>)
    return
  }
  func @kernel_fusable_matmul_ops_num_workgroups__(!shapex.ranked_shape<[?,512]>,
                                                   !shapex.ranked_shape<[512,?]>,
                                                   !shapex.ranked_shape<[?,?]>)
                                                  -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----

module {
  //     CHECK: func @kernel_fusable_pooling()
  //     CHECK:   linalg.fill
  // CHECK-NOT:   return
  //     CHECK:   linalg.pooling
  //     CHECK:   return
  func @kernel_fusable_pooling() attributes {hal.num_workgroups_fn = @kernel_fusable_pooling__num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x?xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<?x?xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x?xf32>
    linalg.fill(%2, %cst) : memref<?x?xf32>, f32
    linalg.pooling_sum(%1, %0, %2) {dilations = [1, 1], strides = [1, 1]} :
      memref<?x?xf32>, memref<?x?xf32>, memref<?x?xf32>
    return
  }
  func @kernel_fusable_pooling__num_workgroups__(!shapex.ranked_shape<[?,?]>,
                                                 !shapex.ranked_shape<[?,?]>,
                                                 !shapex.ranked_shape<[?,?]>)
                                                -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----

// CHECK: module attributes {hal.entry_point_schedule = ["kernel_dispatch_0", "kernel_dispatch_1"]}
module {
  // CHECK: func @kernel_dispatch_1()
  // CHECK:   %[[ZERO:.+]] = constant
  // CHECK:   %[[DIM:.+]] = hal.interface.load.constant
  // CHECK:   %[[SHAPE:.+]] = shapex.make_ranked_shape %[[DIM]]
  // CHECK:   %[[OUT:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
  // CHECK:   %[[TS:.+]] = shapex.tie_shape %[[OUT]], %[[SHAPE]]
  // CHECK:   linalg.fill(%[[TS]], %[[ZERO]])
  // CHECK:   return

  // CHECK: func @kernel_dispatch_0()
  // CHECK:   %[[DIM:.+]] = hal.interface.load.constant
  // CHECK:   %[[SHAPE1:.+]] = shapex.make_ranked_shape %[[DIM]]
  // CHECK:   %[[SHAPE2:.+]] = shapex.make_ranked_shape %[[DIM]]
  // CHECK:   %[[IN1:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x2x2x512xf32>
  // CHECK:   %[[TS1:.+]] = shapex.tie_shape %[[IN1]], %[[SHAPE1]]
  // CHECK:   %[[IN2:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
  // CHECK:   %[[OUT:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
  // CHECK:   %[[TS2:.+]] = shapex.tie_shape %[[OUT]], %[[SHAPE2]]
  // CHECK:   linalg.conv(%[[IN2]], %[[TS1]], %[[TS2]])
  // CHECK:   return

  func @kernel() attributes {hal.num_workgroups_fn = @kernel__num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %dim = hal.interface.load.constant offset = 0 : index
    %shape1 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,2,2,512]>
    %shape2 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,1,1,512]>
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x2x2x512xf32>
    %ts1 = shapex.tie_shape %0, %shape1 : memref<?x2x2x512xf32>, !shapex.ranked_shape<[?,2,2,512]>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
    %ts2 = shapex.tie_shape %2, %shape2 : memref<?x1x1x512xf32>, !shapex.ranked_shape<[?,1,1,512]>
    linalg.conv(%1, %ts1, %ts2) {dilations = [1, 1], padding = dense<[[0, 1], [0, 1]]> : tensor<2x2xi64>, strides = [2, 2]} : memref<3x3x512x1xf32>, memref<?x2x2x512xf32>, memref<?x1x1x512xf32>
    linalg.fill(%ts2, %cst) : memref<?x1x1x512xf32>, f32
    return
  }
  func @kernel__num_workgroups__(!shapex.ranked_shape<[?,2,2,512]>,
                                 !shapex.ranked_shape<[3,3,512,1]>,
                                 !shapex.ranked_shape<[?,1,1,512]>)
                                 -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----

// CHECK: module attributes {hal.entry_point_schedule = ["kernel_dispatch_0", "kernel_dispatch_1", "kernel_dispatch_2"]}
module {
//      CHECK: func @kernel_dispatch_2()
// CHECK-SAME: {hal.num_workgroups_fn = @[[NUM_WORKGROUPS_FN2:.+]]}
//      CHECK:   %[[DIM:.+]] = hal.interface.load.constant
//      CHECK:   %[[SHAPE1:.+]] = shapex.make_ranked_shape %[[DIM]]
//      CHECK:   %[[SHAPE2:.+]] = shapex.make_ranked_shape %[[DIM]]
//      CHECK:   %[[IN1:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x2x2x512xf32>
//      CHECK:   %[[TS1:.+]] = shapex.tie_shape %[[IN1]], %[[SHAPE1]]
//      CHECK:   %[[IN2:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
//      CHECK:   %[[OUT:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
//      CHECK:   %[[TS2:.+]] = shapex.tie_shape %[[OUT]], %[[SHAPE2]]
//      CHECK:   linalg.conv(%[[IN2]], %[[TS1]], %[[TS2]])
//      CHECK:   return

//      CHECK: func private @[[NUM_WORKGROUPS_FN2]]

//      CHECK: func @kernel_dispatch_1()
// CHECK-SAME: {hal.num_workgroups_fn = @[[NUM_WORKGROUPS_FN1:.+]]}
//      CHECK:   %[[C0:.+]] = constant 0 : index
//      CHECK:   %[[C1:.+]] = constant 1 : index
//      CHECK:   scf.parallel (%{{.*}}) = (%[[C0]]) to (%[[C1]]) step (%[[C1]])
//      CHECK:     scf.yield
//      CHECK:   return

//      CHECK: func private @[[NUM_WORKGROUPS_FN1]]

//      CHECK: func @kernel_dispatch_0()
// CHECK-SAME: {hal.num_workgroups_fn = @[[NUM_WORKGROUPS_FN0:.+]]}
//      CHECK:   %[[ZERO:.+]] = constant
//      CHECK:   %[[DIM:.+]] = hal.interface.load.constant
//      CHECK:   %[[SHAPE:.+]] = shapex.make_ranked_shape %[[DIM]]
//      CHECK:   %[[OUT:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
//      CHECK:   %[[TS:.+]] = shapex.tie_shape %[[OUT]], %[[SHAPE]]
//      CHECK:   linalg.fill(%[[TS]], %[[ZERO]])
//      CHECK:   return

//      CHECK: func private @[[NUM_WORKGROUPS_FN0]]

  func @kernel() attributes {hal.num_workgroups_fn = @kernel__num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %c0 = constant 0 : index
    %c1 = constant 1 : index
    %dim = hal.interface.load.constant offset = 0 : index
    %shape1 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,2,2,512]>
    %shape2 = shapex.make_ranked_shape %dim : (index) -> !shapex.ranked_shape<[?,1,1,512]>
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x2x2x512xf32>
    %ts1 = shapex.tie_shape %0, %shape1 : memref<?x2x2x512xf32>, !shapex.ranked_shape<[?,2,2,512]>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x1x1x512xf32>
    %ts2 = shapex.tie_shape %2, %shape2 : memref<?x1x1x512xf32>, !shapex.ranked_shape<[?,1,1,512]>
    linalg.fill(%ts2, %cst) : memref<?x1x1x512xf32>, f32
    scf.parallel (%iv) = (%c0) to (%c1) step (%c1) {
      scf.yield
    }
    linalg.conv(%1, %ts1, %ts2) {dilations = [1, 1], padding = dense<[[0, 1], [0, 1]]> : tensor<2x2xi64>, strides = [2, 2]} : memref<3x3x512x1xf32>, memref<?x2x2x512xf32>, memref<?x1x1x512xf32>
    return
  }
  func @kernel__num_workgroups__(!shapex.ranked_shape<[?,2,2,512]>,
                                 !shapex.ranked_shape<[3,3,512,1]>,
                                 !shapex.ranked_shape<[?,1,1,512]>)
                                 -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----

// Nothing to do if there is just one Linalg op.

// CHECK-NOT: hal.entry_point_schedule
module {
  // CHECK-LABEL: @kernel()
  func @kernel() attributes {hal.num_workgroups_fn = @kernel__num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<1x2x2x512xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<1x1x1x512xf32>
    linalg.conv(%1, %0, %2) {dilations = [1, 1], padding = dense<[[0, 1], [0, 1]]> : tensor<2x2xi64>, strides = [2, 2]} : memref<3x3x512x1xf32>, memref<1x2x2x512xf32>, memref<1x1x1x512xf32>
    return
  }
  // CHECK-LABEL: @kernel__num_workgroups__
  func @kernel__num_workgroups__(!shapex.ranked_shape<[1,2,2,512]>, !shapex.ranked_shape<[3,3,1,512]>, !shapex.ranked_shape<[1,1,1,512]>) -> (index, index, index) attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}



// -----

// Do not split when Linalg and non-Linalg ops are interleaving each other.

module {
  // expected-error @+1 {{cannot separate Linalg/Parallel ops into multiple kernels}}
  func @kernel() {
    %cst = constant 0.000000e+00 : f32
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<1x2x2x512xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<3x3x512x1xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<1x1x1x512xf32>
    linalg.fill(%2, %cst) : memref<1x1x1x512xf32>, f32
    "some_op"() : () -> ()
    linalg.conv(%1, %0, %2) {dilations = [1, 1], padding = dense<[[0, 1], [0, 1]]> : tensor<2x2xi64>, strides = [2, 2]} : memref<3x3x512x1xf32>, memref<1x2x2x512xf32>, memref<1x1x1x512xf32>
    return
  }
  hal.interface @legacy_io attributes {sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

// -----
#map0 = affine_map<(d0, d1) -> (d0 * 12 + d1 + 53)>

module {
  func @subview_interleaved()
  attributes {hal.num_workgroups_fn = @subview_interleaved__num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<18x12xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<12x4xf32>
    linalg.fill(%0, %cst) : memref<18x12xf32>, f32
    %2 = subview %0[4, 5] [18, 12] [1, 1]  : memref<18x12xf32> to memref<18x12xf32, #map0>
    linalg.copy(%1, %2) : memref<12x4xf32>, memref<18x12xf32, #map0>
    return
  }
  func @subview_interleaved__num_workgroups__(!shapex.ranked_shape<[12,4]>,
                                              !shapex.ranked_shape<[18,12]>)
                                              -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=1, type="StorageBuffer", access="Write"
  }
}

//      CHECK: #[[MAP0:.+]] = affine_map<(d0, d1) -> (d0 * 12 + d1 + 53)>
//      CHECK: module attributes {hal.entry_point_schedule =
// CHECK-SAME:   ["subview_interleaved_dispatch_0",
// CHECK-SAME:    "subview_interleaved_dispatch_1"]}
//      CHECK: func @subview_interleaved_dispatch_1()
//  CHECK-DAG:   %[[DST:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<18x12xf32>
//  CHECK-DAG:   %[[SRC:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<12x4xf32>
//      CHECK:   %[[SUB:.+]] = subview %[[DST]][4, 5] [18, 12] [1, 1]  : memref<18x12xf32> to memref<18x12xf32, #[[MAP0]]>
//      CHECK:   linalg.copy(%[[SRC]], %[[SUB]]) : memref<12x4xf32>, memref<18x12xf32, #[[MAP0]]>
//      CHECK:   return
//      CHECK: func @subview_interleaved_dispatch_0()
//      CHECK:   %[[CST:.+]] = constant
//      CHECK:   %[[DST2:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<18x12xf32>
//      CHECK:   linalg.fill(%[[DST2]], %[[CST]]) : memref<18x12xf32>, f32
//      CHECK:   return

// -----

#map0 = affine_map<(d0, d1) -> (d0, d1)>
#map1 = affine_map<(d0, d1, d2) -> (d0, d1)>
#map2 = affine_map<(d0, d1, d2) -> (d2)>

module {
  func @reshape_interleaved()
  attributes {hal.num_workgroups_fn = @reshape_interleaved__num_workgroups__} {
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<2x4xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret1} : memref<1x2x4xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<2x4xf32>
    linalg.generic {indexing_maps = [#map0, #map0],
                    iterator_types = ["parallel", "parallel"]}
      ins(%2 : memref<2x4xf32>)
     outs(%0 : memref<2x4xf32>) {
    ^bb0(%arg0: f32, %arg1: f32):  // no predecessors
      %4 = tanh %arg0 : f32
      linalg.yield %4 : f32
    }
    %3 = linalg.reshape %0 [#map1, #map2] : memref<2x4xf32> into memref<1x2x4xf32>
    linalg.copy(%3, %1) : memref<1x2x4xf32>, memref<1x2x4xf32>
    return
  }
  func @reshape_interleaved__num_workgroups__(!shapex.ranked_shape<[2,4]>,
                                              !shapex.ranked_shape<[2,4]>,
                                              !shapex.ranked_shape<[1,2,4]>)
                                              -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=1, type="StorageBuffer", access="Write|Discard"
    hal.interface.binding @ret1, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}

//  CHECK-DAG: #[[MAP0:.+]] = affine_map<(d0, d1, d2) -> (d0, d1)>
//  CHECK-DAG: #[[MAP1:.+]] = affine_map<(d0, d1, d2) -> (d2)>
//  CHECK-DAG: #[[MAP2:.+]] = affine_map<(d0, d1) -> (d0, d1)>
//      CHECK: module attributes {hal.entry_point_schedule =
// CHECK-SAME:   ["reshape_interleaved_dispatch_0",
// CHECK-SAME:    "reshape_interleaved_dispatch_1"]}
//      CHECK: func @reshape_interleaved_dispatch_1()
//      CHECK:   %[[SRC1:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<2x4xf32>
//      CHECK:   %[[DST:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret1} : memref<1x2x4xf32>
//      CHECK:   %[[SRC2:.+]] = linalg.reshape %[[SRC1]] [#[[MAP0]], #[[MAP1]]] : memref<2x4xf32> into memref<1x2x4xf32>
//      CHECK:   linalg.copy(%[[SRC2]], %[[DST]]) : memref<1x2x4xf32>, memref<1x2x4xf32>
//      CHECK:   return
//      CHECK: func @reshape_interleaved_dispatch_0()
//      CHECK:   %[[OUT:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<2x4xf32>
//      CHECK:   %[[IN:.+]] = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<2x4xf32>
//      CHECK:   linalg.generic
// CHECK-SAME:     ins(%[[IN]] :
// CHECK-SAME:    outs(%[[OUT]] :

// -----

module {
  func @predict_ex_dispatch_0()
  attributes {hal.num_workgroups_fn = @predict_ex_dispatch_0__num_workgroups__} {
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<1x512x1xf32>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret1} : memref<4x8x16xf32>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<1x512x1xf32>
    linalg.copy(%2, %0) : memref<1x512x1xf32>, memref<1x512x1xf32>
    %3 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<4x8x16xf32>
    linalg.generic {indexing_maps = [affine_map<(d0, d1, d2) -> (-d0 + 3, d1, d2)>,
                                     affine_map<(d0, d1, d2) -> (d0, d1, d2)>],
                    iterator_types = ["parallel", "parallel", "parallel"]}
      ins(%3 : memref<4x8x16xf32>)
     outs(%1 : memref<4x8x16xf32>) {
    ^bb0(%arg0: f32, %arg1: f32):  // no predecessors
      linalg.yield %arg0 : f32
    }
    return
  }
  func @predict_ex_dispatch_0__num_workgroups__(!shapex.ranked_shape<[1,512,1]>,
                                                !shapex.ranked_shape<[4,8,16]>,
                                                !shapex.ranked_shape<[1,512,1]>,
                                                !shapex.ranked_shape<[4,8,16]>)
                                                -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}
//      CHECK: module attributes {hal.entry_point_schedule =
// CHECK-SAME:   ["predict_ex_dispatch_0_dispatch_0",
// CHECK-SAME:    "predict_ex_dispatch_0_dispatch_1"]}
//      CHECK: func @predict_ex_dispatch_0_dispatch_1
// CHECK-NEXT:   iree.placeholder
// CHECK-SAME:     binding = @legacy_io::@ret1
// CHECK-NEXT:   iree.placeholder
// CHECK-SAME:     binding = @legacy_io::@arg1
// CHECK-NEXT:   linalg.generic
//      CHECK:     linalg.yield
//  CHECK-NOT:   linalg
//      CHECK:   return
//      CHECK: func @predict_ex_dispatch_0_dispatch_0
// CHECK-NEXT:   iree.placeholder
// CHECK-SAME:     binding = @legacy_io::@ret0
// CHECK-NEXT:   iree.placeholder
// CHECK-SAME:     binding = @legacy_io::@arg0
// CHECK-NEXT:   linalg.copy
//  CHECK-NOT:   linalg
//      CHECK:   return

// -----

module {
  //     CHECK: func @kernel_fusable_fill_matmul_generic_ops
  //     CHECK:   linalg.fill
  // CHECK-NOT:   return
  //     CHECK:   linalg.matmul
  // CHECK-NOT:   return
  //     CHECK:   linalg.generic
  //     CHECK:   return

  func @kernel_fusable_fill_matmul_generic_ops()
  attributes {hal.num_workgroups_fn = @kernel_fusable_fill_matmul_generic_ops_num_workgroups__} {
    %cst = constant 0.000000e+00 : f32
    %dimM = hal.interface.load.constant offset = 0 : index
    %dimN = hal.interface.load.constant offset = 1 : index
    %shape1 = shapex.make_ranked_shape %dimM : (index) -> !shapex.ranked_shape<[?,512]>
    %shape2 = shapex.make_ranked_shape %dimN : (index) -> !shapex.ranked_shape<[512,?]>
    %shape3 = shapex.make_ranked_shape %dimM, %dimN : (index, index) -> !shapex.ranked_shape<[?,?]>
    %0 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg0} : memref<?x512xf32>
    %ts0 = shapex.tie_shape %0, %shape1 : memref<?x512xf32>, !shapex.ranked_shape<[?,512]>
    %1 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg1} : memref<512x?xf32>
    %ts1 = shapex.tie_shape %1, %shape2 : memref<512x?xf32>, !shapex.ranked_shape<[512, ?]>
    %2 = iree.placeholder for "interface buffer" {binding = @legacy_io::@arg2} : memref<?x?xf32>
    %ts2 = shapex.tie_shape %2, %shape3 : memref<?x?xf32>, !shapex.ranked_shape<[?, ?]>
    %3 = iree.placeholder for "interface buffer" {binding = @legacy_io::@ret0} : memref<?x?xf32>
    %ts3 = shapex.tie_shape %3, %shape3 : memref<?x?xf32>, !shapex.ranked_shape<[?,?]>
    %4 = alloc(%dimM, %dimN) : memref<?x?xf32>
    %ts4 = shapex.tie_shape %4, %shape3 : memref<?x?xf32>, !shapex.ranked_shape<[?,?]>
    linalg.fill(%ts4, %cst) : memref<?x?xf32>, f32
    linalg.matmul ins(%ts0, %ts1 : memref<?x512xf32>, memref<512x?xf32>)
                  outs(%ts4 : memref<?x?xf32>)
    linalg.generic
      {indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>,
                        affine_map<(d0, d1) -> (d0, d1)>,
                        affine_map<(d0, d1) -> (d0, d1)>],
       iterator_types = ["parallel", "parallel"]}
      ins(%ts2, %ts4 : memref<?x?xf32>, memref<?x?xf32>)
      outs(%ts3 : memref<?x?xf32>) {
      ^bb0(%arg0 : f32, %arg1 : f32, %arg2 : f32):
        %5 = addf %arg0, %arg1 : f32
        linalg.yield %5 : f32
    }
    return
  }
  func @kernel_fusable_matmul_ops_num_workgroups__(!shapex.ranked_shape<[?,512]>,
                                                   !shapex.ranked_shape<[512,?]>,
                                                   !shapex.ranked_shape<[?,?]>,
                                                   !shapex.ranked_shape<[?,?]>)
                                                  -> (index, index, index)
  attributes {sym_visibility = "private"}
  hal.interface @legacy_io attributes {push_constants = 1 : i32, sym_visibility = "private"} {
    hal.interface.binding @arg0, set=0, binding=0, type="StorageBuffer", access="Read"
    hal.interface.binding @arg1, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @arg2, set=0, binding=1, type="StorageBuffer", access="Read"
    hal.interface.binding @ret0, set=0, binding=2, type="StorageBuffer", access="Write|Discard"
  }
}
