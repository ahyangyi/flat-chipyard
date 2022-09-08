/**********
Copyright (c) 2020, Xilinx, Inc.
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
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

// OpenCL utility layer include
#include "cmdlineparser.h"
#include "xcl2.hpp"
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <unistd.h>
#include <vector>

#define DATA_SIZE 4096
#define INCR_VALUE 10

void p2p_host_to_ssd(int &nvmeFd, cl::Context context, cl::CommandQueue q,
                     cl::Program program,
                     std::vector<int, aligned_allocator<int>> source_input_A) {
  int err;
  int ret = 0, inc = INCR_VALUE;
  int size = DATA_SIZE;
  size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

  cl::Kernel krnl_adder;
  // Allocate Buffer in Global Memory
  cl_mem_ext_ptr_t outExt;
  outExt = {XCL_MEM_EXT_P2P_BUFFER, NULL, 0};

  OCL_CHECK(err,
            cl::Buffer input_a(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
                               vector_size_bytes, source_input_A.data(), &err));
  OCL_CHECK(err,
            cl::Buffer p2pBo(context, CL_MEM_WRITE_ONLY | CL_MEM_EXT_PTR_XILINX,
                             vector_size_bytes, &outExt, &err));
  OCL_CHECK(err, krnl_adder = cl::Kernel(program, "adder", &err));
  // Set the Kernel Arguments
  OCL_CHECK(err, err = krnl_adder.setArg(0, input_a));
  OCL_CHECK(err, err = krnl_adder.setArg(1, p2pBo));
  OCL_CHECK(err, err = krnl_adder.setArg(2, inc));
  OCL_CHECK(err, err = krnl_adder.setArg(3, size));

  // Copy input data to device global memory
  OCL_CHECK(err, err = q.enqueueMigrateMemObjects({input_a},
                                                  0 /* 0 means from host*/));
  // Launch the Kernel
  OCL_CHECK(err, err = q.enqueueTask(krnl_adder));

  std::cout << "\nMap P2P device buffers to host access pointers\n"
            << std::endl;
  void *p2pPtr = q.enqueueMapBuffer(
      p2pBo,                      // buffer
      CL_TRUE,                    // blocking call
      CL_MAP_WRITE | CL_MAP_READ, // Indicates we will be writing
      0,                          // buffer offset
      vector_size_bytes,          // size in bytes
      nullptr, nullptr,
      &err); // error code

  std::cout << "Now start P2P Write from device buffers to SSD\n" << std::endl;
  ret = pwrite(nvmeFd, (void *)p2pPtr, vector_size_bytes, 0);
  if (ret == -1)
    std::cout << "P2P: write() failed, err: " << ret << ", line: " << __LINE__
              << std::endl;

  std::cout << "Clean up the buffers\n" << std::endl;
}

void p2p_ssd_to_host(
    int &nvmeFd, cl::Context context, cl::CommandQueue q, cl::Program program,
    std::vector<int, aligned_allocator<int>> *source_hw_results) {
  int err;
  int inc = INCR_VALUE;
  int size = DATA_SIZE;
  size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

  cl::Kernel krnl_adder1;
  // Allocate Buffer in Global Memory
  cl_mem_ext_ptr_t inExt;
  inExt = {XCL_MEM_EXT_P2P_BUFFER, NULL, 0};

  OCL_CHECK(err, cl::Buffer buffer_input(context, CL_MEM_READ_ONLY |
                                                      CL_MEM_EXT_PTR_XILINX,
                                         vector_size_bytes, &inExt, &err));
  OCL_CHECK(err, cl::Buffer buffer_output(
                     context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                     vector_size_bytes, source_hw_results->data(), &err));
  OCL_CHECK(err, krnl_adder1 = cl::Kernel(program, "adder", &err));
  std::cout << "\nMap P2P device buffers to host access pointers\n"
            << std::endl;
  void *p2pPtr1 =
      q.enqueueMapBuffer(buffer_input,      // buffer
                         CL_TRUE,           // blocking call
                         CL_MAP_READ,       // Indicates we will be writing
                         0,                 // buffer offset
                         vector_size_bytes, // size in bytes
                         nullptr, nullptr,
                         &err); // error code

  std::cout << "Now start P2P Read from SSD to device buffers\n" << std::endl;
  if (pread(nvmeFd, (void *)p2pPtr1, vector_size_bytes, 0) <= 0) {
    std::cerr << "ERR: pread failed: "
              << " error: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  // Set the Kernel Arguments
  OCL_CHECK(err, err = krnl_adder1.setArg(0, buffer_input));
  OCL_CHECK(err, err = krnl_adder1.setArg(1, buffer_output));
  OCL_CHECK(err, err = krnl_adder1.setArg(2, inc));
  OCL_CHECK(err, err = krnl_adder1.setArg(3, size));

  // Launch the Kernel
  OCL_CHECK(err, err = q.enqueueTask(krnl_adder1));

  // Read output data to host
  OCL_CHECK(err, err = q.enqueueReadBuffer(
                     buffer_output, CL_TRUE, 0, vector_size_bytes,
                     source_hw_results->data(), nullptr, nullptr));

  std::cout << "Clean up the buffers\n" << std::endl;
}

int main(int argc, char **argv) {
  // Command Line Parser
  sda::utils::CmdLineParser parser;

  // Switches
  //**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
  parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
  parser.addSwitch("--input_file", "-i", "input test data flie", "");
  parser.parse(argc, argv);

  // Read settings
  auto binaryFile = parser.value("xclbin_file");
  auto filename = parser.value("input_file");

  if (argc != 5) {
    parser.printHelp();
    return EXIT_FAILURE;
  }
  int nvmeFd = -1;
  if (xcl::is_emulation()) {
    filename = "./sample.txt";
  }

  cl_int err;
  cl::Context context;
  cl::CommandQueue q;
  std::vector<int, aligned_allocator<int>> source_input_A(DATA_SIZE, 15);
  std::vector<int, aligned_allocator<int>> source_sw_results(DATA_SIZE);
  std::vector<int, aligned_allocator<int>> source_hw_results(DATA_SIZE);

  // Create the test data and Software Result
  for (int i = 0; i < DATA_SIZE; i++) {
    source_sw_results[i] = source_input_A[i] + 2 * INCR_VALUE;
  }

  // OPENCL HOST CODE AREA START
  // get_xil_devices() is a utility API which will find the xilinx
  // platforms and will return list of devices connected to Xilinx platform
  auto devices = xcl::get_xil_devices();
  // read_binary_file() is a utility API which will load the binaryFile
  // and will return the pointer to file buffer.
  auto fileBuf = xcl::read_binary_file(binaryFile);
  cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
  bool valid_device = false;
  cl::Program program;

  for (unsigned int i = 0; i < devices.size(); i++) {
    auto device = devices[i];
    // Creating Context and Command Queue for selected Device
    OCL_CHECK(err, context = cl::Context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                        CL_QUEUE_PROFILING_ENABLE, &err));
    std::cout << "Trying to program device[" << i
              << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    program = cl::Program(context, {device}, bins, NULL, &err);
    if (err != CL_SUCCESS) {
      std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
    } else {
      std::cout << "Device[" << i << "]: program successful!\n";
      valid_device = true;
      break; // we break because we found a valid device
    }
  }
  if (!valid_device) {
    std::cerr << "Failed to program any device found, exit!\n";
    exit(EXIT_FAILURE);
  }

  // P2P transfer from host to SSD
  std::cout << "############################################################\n";
  std::cout << "                  Writing data to SSD                       \n";
  std::cout << "############################################################\n";
  // Get access to the NVMe SSD.
  nvmeFd = open(filename.c_str(), O_RDWR | O_DIRECT);
  if (nvmeFd < 0) {
    std::cerr << "ERROR: open " << filename << "failed: " << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "INFO: Successfully opened NVME SSD " << filename << std::endl;
  p2p_host_to_ssd(nvmeFd, context, q, program, source_input_A);
  (void)close(nvmeFd);

  // P2P transfer from SSD to host
  std::cout << "############################################################\n";
  std::cout
      << "                  Reading data from SSD                       \n";
  std::cout << "############################################################\n";

  nvmeFd = open(filename.c_str(), O_RDWR | O_DIRECT);
  if (nvmeFd < 0) {
    std::cerr << "ERROR: open " << filename << "failed: " << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "INFO: Successfully opened NVME SSD " << filename << std::endl;

  bool num_matched = true;
  p2p_ssd_to_host(nvmeFd, context, q, program, &source_hw_results);

  // Validating the results
  if (memcmp(static_cast<void *>(source_sw_results.data()),
             static_cast<void *>(source_hw_results.data()), DATA_SIZE) != 0) {
    num_matched = false;
  }

  (void)close(nvmeFd);

  std::cout << "TEST " << (num_matched ? "PASSED" : "FAILED") << std::endl;
  return (num_matched ? EXIT_SUCCESS : EXIT_FAILURE);
}
