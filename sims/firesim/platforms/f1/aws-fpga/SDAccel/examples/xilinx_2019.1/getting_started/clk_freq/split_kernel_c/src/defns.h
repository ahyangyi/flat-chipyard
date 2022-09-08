/**********
Copyright (c) 2018, Xilinx, Inc.
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

#pragma once

#define CHANNELS 3
#define MAX_WIDTH 640
#define MAX_HEIGHT 480

#define WINDOW 3
#define SIZE (WINDOW * WINDOW)
#define BOOST_WINDOW WINDOW
#define MEDIAN_WINDOW WINDOW

#include <cstdlib>
#define imin(X, Y) (((X) < (Y)) ? (X) : (Y))
#define imax(X, Y) (((X) > (Y)) ? (X) : (Y))
#define abs(x) ((x)<0 ? -(x) : (x))

typedef unsigned int uint;

#include "boost_helper.h"
#include "median_helper.h"
#include "sketch_helper.h"

/* The values assigned to the TRIPCOUNT identifiers must be changed as per the input image size. 
 For instance, it has been set for the image with dimenstions 128x128 */
//TRIPCOUNT identifiers
const unsigned int c_width = 128;
const unsigned int c_height = 128;








