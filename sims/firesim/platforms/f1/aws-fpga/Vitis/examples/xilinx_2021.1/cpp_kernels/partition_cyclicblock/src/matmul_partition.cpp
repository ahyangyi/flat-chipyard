/**
* Copyright (C) 2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

// Maximum Matrix Dimension Supported by Kernel
#define MAX_DIM 16

// TRIPCOUNT identifier
const unsigned int c_dim = MAX_DIM;

extern "C" {
void matmul_partition(const int* in1, // Read-Only Matrix 1
                      const int* in2, // Read-Only Matrix 2
                      int* out_r,     // Output Result
                      int dim) {      // Matrix Dimension. Assuming Square Matrix
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = out_r offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = in1
#pragma HLS INTERFACE s_axilite port = in2
#pragma HLS INTERFACE s_axilite port = out_r
#pragma HLS INTERFACE s_axilite port = dim
#pragma HLS INTERFACE s_axilite port = return

    int A[MAX_DIM * MAX_DIM];
    int B[MAX_DIM * MAX_DIM];
    int C[MAX_DIM * MAX_DIM];
// Cyclic Partition for A as matrix multiplication needs row-wise parallel
// access
#pragma HLS ARRAY_PARTITION variable = A dim = 1 cyclic factor = 16
// Block Partition for B as matrix multiplication needs column-wise parallel
// access
#pragma HLS ARRAY_PARTITION variable = B dim = 1 block factor = 16

// As A and B Matrix are partitioned with the factor of MAX_DIM, so to get
// parallel row/column access, input square matrix[dimXdim] should be written
// into local Array in MATRIX[MAX_DIM * MAX_DIM] format

// Burst read for matrix A
// Auto-pipeline is going to apply pipeline to these loops
readA:
    for (int itr = 0, i = 0, j = 0; itr < dim * dim; itr++, j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim* c_dim max = c_dim * c_dim
        if (j == dim) {
            j = 0;
            i++;
        }
        A[i * MAX_DIM + j] = in1[itr];
    }

// Burst read for matrix B
readB:
    for (int itr = 0, i = 0, j = 0; itr < dim * dim; itr++, j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim* c_dim max = c_dim * c_dim
        if (j == dim) {
            j = 0;
            i++;
        }
        B[i * MAX_DIM + j] = in2[itr];
    }

lreorder1:
    for (int i = 0; i < dim; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim max = c_dim
    // As A and B are partition correctly so loop pipelining is applied
    // at 2nd level loop and which will eventually unroll the lower loop
    lreorder2:
        for (int j = 0; j < dim; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim max = c_dim
            int result = 0;
        lreorder3:
            for (int k = 0; k < MAX_DIM; k++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim max = c_dim
                result += A[i * MAX_DIM + k] * B[k * MAX_DIM + j];
            }
            C[i * MAX_DIM + j] = result;
        }
    }

// Burst write from output matrices to global memory
// Burst write from matrix C
writeC:
    for (int itr = 0, i = 0, j = 0; itr < dim * dim; itr++, j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_dim* c_dim max = c_dim * c_dim
        if (j == dim) {
            j = 0;
            i++;
        }
        out_r[itr] = C[i * MAX_DIM + j];
    }
}
}
