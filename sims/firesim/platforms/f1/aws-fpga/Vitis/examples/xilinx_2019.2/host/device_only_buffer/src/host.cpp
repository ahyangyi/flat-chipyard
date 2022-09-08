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
**********/

#include "xcl2.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

const int MAX_DIM = 16;

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];
    cl_int err;
    cl::CommandQueue q;
    cl::Context context;
    cl::Kernel kernel_madd, kernel_mmult;
    const size_t array_size = MAX_DIM * MAX_DIM;
    const size_t size_in_bytes = array_size * sizeof(int);

    // allocate memory on host for input and output matrices
    std::vector<int, aligned_allocator<int>> A(array_size);
    std::vector<int, aligned_allocator<int>> B(array_size);
    std::vector<int, aligned_allocator<int>> sw_results(array_size, 0);
    std::vector<int, aligned_allocator<int>> C(array_size, 1);
    std::vector<int, aligned_allocator<int>> hw_results(array_size, 0);

    //Create test data
    for (int i = 0; i < int(array_size); i++) {
        A[i] = i;
        B[i] = 2 * i;
    }
    //generating sw_results
    for (int i = 0; i < MAX_DIM; i++) {
        for (int j = 0; j < MAX_DIM; j++) {
            for (int k = 0; k < MAX_DIM; k++) {
                sw_results[i * MAX_DIM + j] +=
                    A[i * MAX_DIM + k] * B[k * MAX_DIM + j];
            }
        }
    }

    for (int i = 0; i < MAX_DIM * MAX_DIM; i++)
        sw_results[i] += C[i];

    // The get_xil_devices will return std::vector of Xilinx Devices
    // platforms and will return list of devices connected to Xilinx platform
    auto devices = xcl::get_xil_devices();

    // read_binary_file() is a utility API which will load the binaryFile
    // and will return pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    int valid_device = 0;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context({device}, NULL, NULL, NULL, &err));
        OCL_CHECK(err,
                  q = cl::CommandQueue(
                      context, {device}, CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i
                  << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, NULL, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i
                      << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, kernel_madd = cl::Kernel(program, "madd", &err));
            OCL_CHECK(err, kernel_mmult = cl::Kernel(program, "mmult", &err));
            valid_device++;
            break; // we break because we found a valid device
        }
    }
    if (valid_device == 0) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    // create normal buffers
    OCL_CHECK(err,
              cl::Buffer buffer_a(context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                                  size_in_bytes,
                                  A.data(),
                                  &err));
    OCL_CHECK(err,
              cl::Buffer buffer_b(context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                                  size_in_bytes,
                                  B.data(),
                                  &err));

    OCL_CHECK(err,
              cl::Buffer buffer_c(context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                                  size_in_bytes,
                                  C.data(),
                                  &err));
    OCL_CHECK(err,
              cl::Buffer buffer_d(context,
                                  CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,
                                  size_in_bytes,
                                  hw_results.data(),
                                  &err));

    // creating device only buffer, buffer is not mapped to Host and is only accessed by the kernels.
    // CL_MEM_HOST_NO_ACCESS flag specifies that host machine will not read/write the memory object.
    OCL_CHECK(err,
              cl::Buffer dev_only_buf(context,
                                      CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS,
                                      size_in_bytes,
                                      NULL,
                                      &err));

    // set kernel arguments for 1st kernel :   output to device only buffer
    OCL_CHECK(err, err = kernel_mmult.setArg(0, buffer_a));
    OCL_CHECK(err, err = kernel_mmult.setArg(1, buffer_b));
    OCL_CHECK(err, err = kernel_mmult.setArg(2, dev_only_buf));
    OCL_CHECK(err, err = kernel_mmult.setArg(3, MAX_DIM));

    // set kernel arguments for 2nd kernel : input from device only buffer
    OCL_CHECK(err, err = kernel_madd.setArg(0, dev_only_buf));
    OCL_CHECK(err, err = kernel_madd.setArg(1, buffer_c));
    OCL_CHECK(err, err = kernel_madd.setArg(2, buffer_d));
    OCL_CHECK(err, err = kernel_madd.setArg(3, MAX_DIM));

    //migrate data to global memory
    OCL_CHECK(
        err,
        err = q.enqueueMigrateMemObjects({buffer_a, buffer_b, buffer_c}, 0));

    // execute 1st and 2nd kernels : since data is required for 2nd kernel from 1st , use events or use in order queue
    OCL_CHECK(err, err = q.enqueueTask(kernel_mmult));
    OCL_CHECK(err, err = q.enqueueTask(kernel_madd));

    // take results
    OCL_CHECK(err,
              err = q.enqueueMigrateMemObjects({buffer_d},
                                               CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.finish());

    // verify
    bool match =
        std::equal(sw_results.begin(), sw_results.end(), hw_results.begin());
    std::cout << std::endl
              << "TEST " << (match ? "PASSED" : "FAILED") << std::endl;

    return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}
