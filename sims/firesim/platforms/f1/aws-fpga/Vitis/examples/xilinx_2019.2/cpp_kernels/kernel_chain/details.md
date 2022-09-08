Kernel Chain Matrix Multiplication (C)
======================================
This examples demonstrates how 'ap_ctrl_chain' in HLS Kernel can help to improve the performance.
In this example matrix multiplication functionality is used to showcase the benefit. Example contains two kernels
1. krnl_chain_mmult: Showcases the Kernel with 'ap_ctrl_chain' functionality
2. krnl_simple_mmult: Same kernel without 'ap_ctrl_chain'

Kernel consists of following sub-blocks
1. mm2s: converts AXIMM data from memory to AXIS data
2. mmult: compute for matrix multplication
2. s2mm: converts AXIS data from compute kernel to AXIMM

ap_ctrl_chain implements a set of block-level control ports to start the design operation, continue operation, and
indicate when the design is idle, done, and ready for new input data. The ap_ctrl_chain interface mode is similar to ap_ctrl_hs but provides an additional input signal ap_continue to apply back pressure. Xilinx recommends using the ap_ctrl_chain block-level I/O protocol when chaining Vivado HLS blocks together.

This examples shows the performance difference between the chain kernel and simple kernel.

```c++
#pragma HLS INTERFACE s_axilite port = return bundle = control
#pragma HLS INTERFACE ap_ctrl_chain port = return bundle = control
```
