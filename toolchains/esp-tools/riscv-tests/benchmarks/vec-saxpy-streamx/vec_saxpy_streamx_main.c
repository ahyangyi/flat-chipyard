// See LICENSE for license details.

//**************************************************************************
// Vector SAXPY benchmark for Hwacha v4
//--------------------------------------------------------------------------
//

#include "util.h"
#include "vec_saxpy_streamx.h"

//--------------------------------------------------------------------------
// Input/Reference Data

#include "dataset1.h"

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  // Do the saxpy_streamx
  setStats(1);
  for (int i = 0; i < NUM_STREAMS; i++) {
      vec_saxpy_streamx_asm(DATA_SIZE, input_data_a, input_data_X_streams[i], input_data_Y);
  }
  setStats(0);

  // Check the results
  return verifyFloat(DATA_SIZE, input_data_Y, verify_data);
}
