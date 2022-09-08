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

/*******************************************************************************
Description:
    This example demonstrate to utilized both DDR and full bandwidth using
    watermark Application. In Watermark application, kernel has to apply a fixed
    watermark (Here is it Xilinx First Character 'X') into a given Image and
    write the output image.
    Inside Host code, input image is placed into Bank0 and Kernel will read
    input Image and write the output image to Bank1.
    To utilized the both Banks fully Kernel code do burst read input image with
    full datawidth of 512 and do the burst write of output image with full
    datawidth of 512.
    As Kernel is accessing Sequentially from both the DDR, so kernel with get
the
    Best memory access bandwidth from both DDRs and will do watermark with good
    performance.
*******************************************************************************/

// Water Mark Defines
#define CHANNELS 3 // Three Channels (R, G and B)
#define WATERMARK_HEIGHT 16
#define WATERMARK_WIDTH 16

// Using Datatype uint16 to get the full memory datawidth 512
#define TYPE uint16

// Per Memory Access getting 16 pixels
#define DATA_SIZE 16

// Tripcount identifiers
__constant int c_size = DATA_SIZE;

// function declaration
int saturatedAdd(int x, int y);

__kernel __attribute__((reqd_work_group_size(1, 1, 1))) void apply_watermark(__global const TYPE* __restrict input,
                                                                             __global TYPE* __restrict output,
                                                                             int width,
                                                                             int height) {
    // WaterMark Image of 16x16 size
    int watermark[WATERMARK_HEIGHT][WATERMARK_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0},
        {0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0},
        {0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0},
        {0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0},
        {0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0},
        {0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0},
        {0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    uint imageSize = width * height; // Total Number of Pixels
    // As memory access is int16 type so total 16 pixels can be access at a time,
    // so calculating total number of Memory accesses which are needed to entire
    // Image
    uint size = ((imageSize - 1) / DATA_SIZE) + 1;

    // Process the whole image
    __attribute__((xcl_pipeline_loop(1))) image_traverse
        : for (uint idx = 0, x = 0, y = 0; idx < size; ++idx, x += DATA_SIZE) {
        // Read the next 16 Pixels
        TYPE tmp = input[idx];

        // Row Boundary Check for x
        if (x >= width) {
            x = x - width;
            ++y;
        }
        // Unrolling below loop to process all 16 pixels concurrently
        __attribute__((opencl_unroll_hint)) __attribute__((xcl_loop_tripcount(c_size, c_size))) watermark
            : for (int i = 0; i < DATA_SIZE; i++) {
            uint tmp_x = x + i;
            uint tmp_y = y;
            // Row Boundary Check for x
            if (tmp_x > width) {
                tmp_x = tmp_x - width;
                tmp_y += 1;
            }

            uint w_idy = tmp_y % WATERMARK_HEIGHT;
            uint w_idx = tmp_x % WATERMARK_WIDTH;
            tmp[i] = saturatedAdd(tmp[i], watermark[w_idy][w_idx]);
        }

        // Write the Next 16 Pixels result to output memory
        output[idx] = tmp;
    }
}

int saturatedAdd(int x, int y) {
    // Separate into the different channels

    // Red Channel
    int redX = x & 0xff;
    int redY = y & 0xff;
    int redOutput;

    // Green Channel
    int greenX = (x & 0xff00) >> 8;
    int greenY = (y & 0xff00) >> 8;
    int greenOutput;

    // Blue Channel
    int blueX = (x & 0xff0000) >> 16;
    int blueY = (y & 0xff0000) >> 16;
    int blueOutput;

    // Calculating Red
    if (redX + redY > 255) {
        redOutput = 255;
    } else {
        redOutput = redX + redY;
    }

    // Calculating Green
    if (greenX + greenY > 255) {
        greenOutput = 255;
    } else {
        greenOutput = greenX + greenY;
    }

    // Calculating Blue
    if (blueX + blueY > 255) {
        blueOutput = 255;
    } else {
        blueOutput = blueX + blueY;
    }

    // Combining all channels into one
    int combinedOutput = 0;
    combinedOutput |= redOutput;
    combinedOutput |= (greenOutput << 8);
    combinedOutput |= (blueOutput << 16);
    return combinedOutput;
}
