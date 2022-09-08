/**********
Copyright (c) 2019, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This is simple Example of Multiple Compute units to showcase how a single kernel
can be instantiated into Multiple compute units. Host code will show how to use 
multiple compute units and run them concurrently.
*************************************************/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

//OpenCL utility layer include
#include "xcl2.hpp"

auto constexpr data_size = 4 * 4096;
auto constexpr num_cu = 4;

//////////////MAIN FUNCTION//////////////
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << "<XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string binaryFile = argv[1];
    cl_int err;

    // I/O data vectors
    std::vector<int, aligned_allocator<int>> source_in1(data_size);
    std::vector<int, aligned_allocator<int>> source_in2(data_size);
    std::vector<int, aligned_allocator<int>> source_hw_results(data_size);
    std::vector<int> sw_results(data_size);
    
    //Create test data
    std::generate(source_in1.begin(), source_in1.end(), std::rand);
    std::generate(source_in2.begin(), source_in2.end(), std::rand);
    for (int i = 0; i < data_size; i++) {
        sw_results[i] = source_in1[i] + source_in2[i];
    }

    // get_xil_devices() is a utility API which will find the xilinx
    // platforms and will return list of devices connected to Xilinx platform
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    //Creating context
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));

    //Creating command queue
    OCL_CHECK(err,
              cl::CommandQueue q(context,
                                 device,
                                 CL_QUEUE_PROFILING_ENABLE |
                                     CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                                 &err));

    //read_binary_file() is a utility API which will load the binaryFile
    //and will return the pointer to file buffer.
   auto fileBuf = xcl::read_binary_file(binaryFile);
   cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    devices.resize(1);

    // Creating Program
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));

    //Creating Kernel objects
    std::vector<cl::Kernel> krnls(num_cu);
    for (int i = 0; i < num_cu; i++) {
        OCL_CHECK(err, krnls[i] = cl::Kernel(program, "vadd", &err));
    }

    //Creating buffers
    auto chunk_size = data_size / num_cu;
    size_t vector_size_bytes = sizeof(int) * chunk_size;
    std::vector<cl::Buffer> buffer_in1(num_cu);
    std::vector<cl::Buffer> buffer_in2(num_cu);
    std::vector<cl::Buffer> buffer_output(num_cu);

    for (int i = 0; i < num_cu; i++) {
        OCL_CHECK(err,
                  buffer_in1[i] =
                      cl::Buffer(context,
                                 CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                                 vector_size_bytes,
                                 source_in1.data() + i * chunk_size,
                                 &err));
        OCL_CHECK(err,
                  buffer_in2[i] =
                      cl::Buffer(context,
                                 CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                                 vector_size_bytes,
                                 source_in2.data() + i * chunk_size,
                                 &err));
        OCL_CHECK(err,
                  buffer_output[i] =
                      cl::Buffer(context,
                                 CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
                                 vector_size_bytes,
                                 source_hw_results.data() + i * chunk_size,
                                 &err));
    }

    for (int i = 0; i < num_cu; i++) {
        int narg = 0;

        //Setting kernel arguments
        OCL_CHECK(err, err = krnls[i].setArg(narg++, buffer_in1[i]));
        OCL_CHECK(err, err = krnls[i].setArg(narg++, buffer_in2[i]));
        OCL_CHECK(err, err = krnls[i].setArg(narg++, buffer_output[i]));
        OCL_CHECK(err, err = krnls[i].setArg(narg++, chunk_size));

        //Copy input data to device global memory
        OCL_CHECK(err,
                  err =
                      q.enqueueMigrateMemObjects({buffer_in1[i], buffer_in2[i]},
                                                 0 /* 0 means from host*/));

        //Launch the kernel
        OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
    }

    OCL_CHECK(err, err = q.finish());

    //Copy result from device global memory to host local memory
    for (int i = 0; i < num_cu; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects({buffer_output[i]},
                                                   CL_MIGRATE_MEM_OBJECT_HOST));
    }
    OCL_CHECK(err, err = q.finish());

    //Verify results
    bool match = std::equal(
        sw_results.begin(), sw_results.end(), source_hw_results.begin());
    std::cout << "TEST " << (match ? "PASSED" : "FAILED") << std::endl;

    return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}
