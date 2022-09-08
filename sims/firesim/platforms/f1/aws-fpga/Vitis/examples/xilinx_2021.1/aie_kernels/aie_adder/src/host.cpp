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

#include "aie_graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>

// This is used for the PL Kernels
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
simpleGraph addergraph;

static std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm) {
    if (fnm.empty()) throw std::runtime_error("No xclbin specified");

    // load bit stream
    std::ifstream stream(fnm);
    stream.seekg(0, stream.end);
    size_t size = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<char> header(size);
    stream.read(header.data(), size);

    auto top = reinterpret_cast<const axlf*>(header.data());
    if (xrtDeviceLoadXclbin(device, top)) throw std::runtime_error("Xclbin loading failed");

    return header;
}

#define DATA_SIZE 1024
#define N_ITER DATA_SIZE / 4

int main(int argc, char** argv) {
    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////
    auto dhdl = xrtDeviceOpen(0); // Open Device the local device
    if (dhdl == nullptr) throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin = load_xclbin(dhdl, "krnl_adder.xclbin");
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);

    int sizeIn = DATA_SIZE;
    int sizeOut = DATA_SIZE;

    int DataInput0[sizeIn], DataInput1[sizeIn];
    int golden[sizeOut];

    for (int i = 0; i < sizeIn; i++) {
        DataInput0[i] = rand() % 100;
        DataInput1[i] = rand() % 100;
    }

    //////////////////////////////////////////
    // input memory
    // Allocating the input size of sizeIn to MM2S
    // This is using low-level XRT call xclAllocBO to allocate the memory
    //////////////////////////////////////////

    xrtBufferHandle in_bohdl0 = xrtBOAlloc(dhdl, sizeIn * sizeof(int), 0, 0);
    auto in_bomapped0 = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl0));
    memcpy(in_bomapped0, DataInput0, sizeIn * sizeof(int));
    printf("Input memory virtual addr 0x%px\n", in_bomapped0);

    xrtBufferHandle in_bohdl1 = xrtBOAlloc(dhdl, sizeIn * sizeof(int), 0, 0);
    auto in_bomapped1 = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl1));
    memcpy(in_bomapped1, DataInput1, sizeIn * sizeof(int));
    printf("Input memory virtual addr 0x%px\n", in_bomapped1);

    //////////////////////////////////////////
    // output memory
    // Allocating the output size of sizeOut to S2MM
    // This is using low-level XRT call xclAllocBO to allocate the memory
    //////////////////////////////////////////

    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, sizeOut * sizeof(int), 0, 0);
    auto out_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(out_bohdl));
    memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(int));
    printf("Output memory virtual addr 0x%px\n", out_bomapped);

    //////////////////////////////////////////
    // mm2s ip
    // Using the xrtPLKernelOpen function to manually control the PL Kernel
    // that is outside of the AI Engine graph
    //////////////////////////////////////////

    xrtKernelHandle mm2s_khdl1 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "pl_mm2s:{pl_mm2s_1}");
    // Need to provide the kernel handle, and the argument order of the kernel arguments
    // Here the in_bohdl is the input buffer, the nullptr is the streaming interface and must be null,
    // lastly, the size of the data. This info can be found in the kernel definition.
    xrtRunHandle mm2s_rhdl1 = xrtKernelRun(mm2s_khdl1, in_bohdl0, nullptr, sizeIn);
    printf("run pl_mm2s_1\n");

    xrtKernelHandle mm2s_khdl2 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "pl_mm2s:{pl_mm2s_2}");
    xrtRunHandle mm2s_rhdl2 = xrtKernelRun(mm2s_khdl2, in_bohdl1, nullptr, sizeIn);
    printf("run pl_mm2s_2\n");

    //////////////////////////////////////////
    // s2mm ip
    // Using the xrtPLKernelOpen function to manually control the PL Kernel
    // that is outside of the AI Engine graph
    //////////////////////////////////////////

    xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "pl_s2mm");
    // Need to provide the kernel handle, and the argument order of the kernel arguments
    // Here the out_bohdl is the output buffer, the nullptr is the streaming interface and must be null,
    // lastly, the size of the data. This info can be found in the kernel definition.
    xrtRunHandle s2mm_rhdl = xrtKernelRun(s2mm_khdl, out_bohdl, nullptr, sizeOut);
    printf("run pl_s2mm\n");

    //////////////////////////////////////////
    // graph execution for AIE
    //////////////////////////////////////////

    printf("graph init. This does nothing because CDO in boot PDI already configures AIE.\n");
    addergraph.init();

    printf("graph run\n");
    addergraph.run(N_ITER);

    addergraph.end();
    printf("graph end\n");

    //////////////////////////////////////////
    // wait for mm2s done
    //////////////////////////////////////////

    auto state = xrtRunWait(mm2s_rhdl1);
    std::cout << "mm2s_1 completed with status(" << state << ")\n";
    xrtRunClose(mm2s_rhdl1);
    xrtKernelClose(mm2s_khdl1);

    state = xrtRunWait(mm2s_rhdl2);
    std::cout << "mm2s_2 completed with status(" << state << ")\n";
    xrtRunClose(mm2s_rhdl2);
    xrtKernelClose(mm2s_khdl2);

    //////////////////////////////////////////
    // wait for s2mm done
    //////////////////////////////////////////

    state = xrtRunWait(s2mm_rhdl);
    std::cout << "s2mm completed with status(" << state << ")\n";
    xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);

    //////////////////////////////////////////
    // Comparing the execution data to the golden data
    //////////////////////////////////////////

    int errorCount = 0;
    {
        for (int i = 0; i < sizeOut; i++) {
            golden[i] = DataInput0[i] + DataInput1[i];
            if ((signed)out_bomapped[i] != golden[i]) {
                printf("Error found @ %d, %d != %d\n", i, out_bomapped[i], golden[i]);
                errorCount++;
            }
        }

        if (errorCount)
            printf("Test failed with %d errors\n", errorCount);
        else
            printf("TEST PASSED\n");
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << "Releasing remaining XRT objects...\n";
    xrtBOFree(in_bohdl0);
    xrtBOFree(in_bohdl1);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

    return errorCount;
}
