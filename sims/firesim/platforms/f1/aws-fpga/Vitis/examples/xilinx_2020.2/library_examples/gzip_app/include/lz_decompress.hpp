/*
 * (c) Copyright 2019 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _XFCOMPRESSION_LZ_DECOMPRESS_HPP_
#define _XFCOMPRESSION_LZ_DECOMPRESS_HPP_

/**
 * @file lz_decompress.hpp
 * @brief Header for modules used in LZ4 and snappy decompression kernels.
 *
 * This file is part of Vitis Data Compression Library.
 */

#include "hls_stream.h"

#include <ap_int.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

namespace xf {
namespace compression {

template <typename T> T reg(T d) {
#pragma HLS PIPELINE II = 1
#pragma HLS INTERFACE ap_ctrl_none register port = return
#pragma HLS INLINE off
  return d;
}

/**
 * @brief This module writes the literals to the output stream as it is
 * and when match length and offset are read, the literals will be read from
 * the local dictionary based on offset until match length.
 *
 * @tparam LOW_OFFSET low offset
 * @tparam HISTORY_SIZE history size
 *
 * @param inStream input stream
 * @param outStream output stream
 * @param original_size original size
 */
template <int HISTORY_SIZE, int LOW_OFFSET = 8>
void lzDecompress(hls::stream<ap_uint<32>> &inStream,
                  hls::stream<ap_uint<8>> &outStream, uint32_t original_size) {
  enum lzDecompressStates { READ_STATE, MATCH_STATE, LOW_OFFSET_STATE };

  uint8_t local_buf[HISTORY_SIZE];
#pragma HLS dependence variable = local_buf inter false

  uint32_t match_len = 0;
  uint32_t out_len = 0;
  uint32_t match_loc = 0;
  uint32_t length_extract = 0;
  enum lzDecompressStates next_states = READ_STATE;
  uint16_t offset = 0;
  ap_uint<32> nextValue;
  ap_uint<8> outValue = 0;
  ap_uint<8> prevValue[LOW_OFFSET];
#pragma HLS ARRAY_PARTITION variable = prevValue dim = 0 complete
lz_decompress:
  for (uint32_t i = 0; i < original_size; i++) {
#pragma HLS PIPELINE II = 1
    if (next_states == READ_STATE) {
      nextValue = inStream.read();
      offset = nextValue.range(15, 0);
      length_extract = nextValue.range(31, 16);
      if (length_extract) {
        match_loc = i - offset - 1;
        match_len = length_extract + 1;
        out_len = 1;
        if (offset >= LOW_OFFSET) {
          next_states = MATCH_STATE;
          outValue = local_buf[match_loc % HISTORY_SIZE];
        } else {
          next_states = LOW_OFFSET_STATE;
          outValue = prevValue[offset];
        }
        match_loc++;
      } else {
        outValue = nextValue.range(7, 0);
      }
    } else if (next_states == LOW_OFFSET_STATE) {
      outValue = prevValue[offset];
      match_loc++;
      out_len++;
      if (out_len == match_len)
        next_states = READ_STATE;
    } else {
      outValue = local_buf[match_loc % HISTORY_SIZE];
      match_loc++;
      out_len++;
      if (out_len == match_len)
        next_states = READ_STATE;
    }
    local_buf[i % HISTORY_SIZE] = outValue;
    outStream << outValue;
    for (uint32_t pIdx = LOW_OFFSET - 1; pIdx > 0; pIdx--) {
#pragma HLS UNROLL
      prevValue[pIdx] = prevValue[pIdx - 1];
    }
    prevValue[0] = outValue;
  }
}

/**
 * @brief This module writes the literals to the output stream as it is
 * and when match length and offset are read, the literals will be read from
 * the local dictionary based on offset until match length. This module can
 * process data in parallel defined by PARALLEL_BYTES template argument
 *
 * @tparam PARALLEL_BYTES number of bytes processed in parallel (4, 8)
 * @tparam HISTORY_SIZE history size
 * @tparam SIZE_DT input data type
 *
 * @param litlenStream literal length stream
 * @param litStream literals only stream
 * @param offsetStream offset only stream
 * @param matchlenStream match length only stream
 * @param outStream output stream
 * @param endOfStream end of stream
 * @param sizeOutStream output size stream
 */
template <int PARALLEL_BYTES, int HISTORY_SIZE, class SIZE_DT = uint8_t>
void lzMultiByteDecompress(hls::stream<SIZE_DT> &litlenStream,
                           hls::stream<ap_uint<PARALLEL_BYTES * 8>> &litStream,
                           hls::stream<ap_uint<16>> &offsetStream,
                           hls::stream<SIZE_DT> &matchlenStream,
                           hls::stream<ap_uint<PARALLEL_BYTES * 8>> &outStream,
                           hls::stream<bool> &endOfStream,
                           hls::stream<uint64_t> &sizeOutStream) {
  const uint8_t c_parallelBit = PARALLEL_BYTES * 8;
  const uint8_t c_lowOffset = 4 * PARALLEL_BYTES;
  const uint8_t c_veryLowOffset = 2 * PARALLEL_BYTES;

  const uint16_t c_ramHistSize = HISTORY_SIZE / PARALLEL_BYTES;
  const uint8_t c_regHistSize = (2 * c_lowOffset) / PARALLEL_BYTES;

  enum lzDecompressStates {
    READ_LIT_LEN,
    WRITE_LITERAL,
    READ_OFFSET,
    READ_MATCH,
    NO_OP
  };
  enum lzDecompressStates next_state =
      READ_LIT_LEN; // start from Read Literal Length

  ap_uint<c_parallelBit> ramHistory[2][c_ramHistSize];
#pragma HLS dependence variable = ramHistory inter false
#pragma HLS resource variable = ramHistory core = RAM_2P_URAM
#pragma HLS ARRAY_PARTITION variable = ramHistory dim = 1 complete

  ap_uint<c_parallelBit> regHistory[2][c_regHistSize];
// full partition  to infer as reg
#pragma HLS ARRAY_PARTITION variable = regHistory dim = 0 complete

  SIZE_DT lit_len = 0;
  SIZE_DT orig_lit_len = 0;
  uint32_t output_cnt = 0;
  uint16_t match_loc = 0;
  SIZE_DT match_len = 0;
  uint16_t write_idx = 0;
  uint16_t output_index = 0;
  uint64_t outSize = 0;

  ap_uint<2 * c_parallelBit> regLocalValue;
  ap_uint<2 * c_parallelBit> ramLocalValue;

  uint8_t incr_output_index = 0;
  bool outStreamFlag = false;

  ap_uint<16> offset = 0;
  ap_uint<c_parallelBit> outStreamValue = 0;
  ap_uint<2 * PARALLEL_BYTES * 8> output_window;
  uint8_t parallelBits = 0;

  bool matchDone = false;

lz_decompress:
  for (; matchDone == false;) {
#pragma HLS PIPELINE II = 1
    uint16_t read_idx = match_loc / PARALLEL_BYTES;
    uint16_t byte_loc = match_loc % PARALLEL_BYTES;

    // always reading to make better timing
    ap_uint<c_parallelBit> lowValueReg =
        regHistory[0][(read_idx + 0) % c_regHistSize];
    ap_uint<c_parallelBit> highValueReg =
        regHistory[1][(read_idx + 1) % c_regHistSize];
    ap_uint<c_parallelBit> lowValueRam =
        ramHistory[0][(read_idx + 0) % c_ramHistSize];
    ap_uint<c_parallelBit> highValueRam =
        ramHistory[1][(read_idx + 1) % c_ramHistSize];

    regLocalValue.range(c_parallelBit - 1, 0) = lowValueReg;
    regLocalValue.range(2 * c_parallelBit - 1, c_parallelBit) = highValueReg;

    ramLocalValue.range(c_parallelBit - 1, 0) = lowValueRam;
    ramLocalValue.range(2 * c_parallelBit - 1, c_parallelBit) = highValueRam;

    if (next_state == READ_LIT_LEN) {
      incr_output_index = 0;
      orig_lit_len = litlenStream.read();
      lit_len = orig_lit_len;
      if (lit_len) {
        next_state = WRITE_LITERAL;
      } else {
        next_state = READ_OFFSET;
      }
      output_cnt += lit_len;
    } else if (next_state == WRITE_LITERAL) {
      ap_uint<c_parallelBit> input = litStream.read();
      output_window.range((output_index + PARALLEL_BYTES) * 8 - 1,
                          output_index * 8) = input;
      if (lit_len >= PARALLEL_BYTES) {
        incr_output_index = PARALLEL_BYTES;
        lit_len -= PARALLEL_BYTES;
      } else {
        incr_output_index = lit_len;
        lit_len = 0;
      }
      if (lit_len) {
        next_state = WRITE_LITERAL;
      } else {
        offset = offsetStream.read();
        match_len = matchlenStream.read();
        match_loc = output_cnt - offset;
        if (orig_lit_len == 0 && match_len == 0) {
          matchDone = true;
        } else if (match_len == 0) {
          next_state = READ_LIT_LEN;
        } else if ((offset > 0) & (offset < c_veryLowOffset)) {
          parallelBits = 1;
          if (offset < PARALLEL_BYTES) {
            next_state = NO_OP;
          } else {
            next_state = READ_MATCH;
          }
        } else {
          parallelBits = PARALLEL_BYTES;
          next_state = READ_MATCH;
        }
        output_cnt += match_len;
      }
    } else if (next_state == READ_OFFSET) {
      incr_output_index = 0;
      offset = offsetStream.read();
      match_len = matchlenStream.read();
      match_loc = output_cnt - offset;
      if (orig_lit_len == 0 && match_len == 0) {
        matchDone = true;
      } else if (match_len == 0) {
        next_state = READ_LIT_LEN;
      } else if ((offset > 0) & (offset < c_veryLowOffset)) {
        parallelBits = 1;
        next_state = READ_MATCH;
      } else {
        parallelBits = PARALLEL_BYTES;
        next_state = READ_MATCH;
      }
      output_cnt += match_len;
    } else if (next_state == READ_MATCH) {
      if (offset < c_lowOffset) {
        output_window.range((output_index + PARALLEL_BYTES) * 8 - 1,
                            output_index * 8) =
            regLocalValue.range(
                ((byte_loc % PARALLEL_BYTES) + PARALLEL_BYTES) * 8 - 1,
                (byte_loc % PARALLEL_BYTES) * 8);
      } else {
        output_window.range((output_index + PARALLEL_BYTES) * 8 - 1,
                            output_index * 8) =
            ramLocalValue.range(
                ((byte_loc % PARALLEL_BYTES) + PARALLEL_BYTES) * 8 - 1,
                (byte_loc % PARALLEL_BYTES) * 8);
      }

      if (match_len >= parallelBits) {
        incr_output_index = parallelBits;
        match_loc += parallelBits;
        match_len -= parallelBits;
      } else {
        incr_output_index = match_len;
        match_loc += match_len;
        match_len = 0;
      }
      if (match_len) {
        next_state = READ_MATCH;
      } else {
        orig_lit_len = litlenStream.read();
        lit_len = orig_lit_len;
        if (lit_len) {
          next_state = WRITE_LITERAL;
        } else {
          next_state = READ_OFFSET;
        }
        output_cnt += lit_len;
      }
    } else if (next_state == NO_OP) {
      incr_output_index = 0;
      // Adding NO_OP as workaround for low offset case as
      // for very low offset case, results are not matching
      next_state = READ_MATCH;
    } else {
      assert(0);
    }
    outStreamValue = output_window.range(c_parallelBit - 1, 0);

    regHistory[0][write_idx % c_regHistSize] = outStreamValue;
    regHistory[1][write_idx % c_regHistSize] = outStreamValue;

    if ((output_index + incr_output_index) >= PARALLEL_BYTES) {
      outStreamFlag = true;
      ramHistory[0][write_idx % c_ramHistSize] = outStreamValue;
      ramHistory[1][write_idx % c_ramHistSize] = outStreamValue;

      write_idx++;
      output_window >>= PARALLEL_BYTES * 8;
      output_index += incr_output_index - PARALLEL_BYTES;
    } else {
      outStreamFlag = false;
      output_index += incr_output_index;
    }

    if (outStreamFlag) {
      outStream << outStreamValue;
      endOfStream << 0;
      outSize += PARALLEL_BYTES;
    }
  }

  // output_index:%d\n",lit_len,match_len,incr_output_index,output_index);
  // Write out if there is remaining left over data in output buffer
  // to outStream
  if (output_index) {
    outStreamValue = output_window.range(c_parallelBit - 1, 0);
    outStream << outStreamValue;
    endOfStream << 0;
    outSize += output_index;
  }

  outStream << 0;
  endOfStream << 1;
  sizeOutStream << outSize;
}

template <int PARALLEL_BYTES, int HISTORY_SIZE, class SIZE_DT = uint16_t>
void lzMultiByteDecoder(hls::stream<SIZE_DT> &litlenStream,
                        hls::stream<ap_uint<PARALLEL_BYTES * 8>> &litStream,
                        hls::stream<ap_uint<16>> &offsetStream,
                        hls::stream<SIZE_DT> &matchlenStream,
                        hls::stream<ap_uint<PARALLEL_BYTES * 8>> &outStream,
                        hls::stream<bool> &endOfStream,
                        hls::stream<uint32_t> &sizeOutStream) {
  const uint8_t c_parallelBit = PARALLEL_BYTES * 8;
  const uint8_t c_lowOffset = 8 * PARALLEL_BYTES;
  const uint8_t c_veryLowOffset = 2 * PARALLEL_BYTES;

  const uint16_t c_ramHistSize = HISTORY_SIZE / PARALLEL_BYTES;
  const uint8_t c_regHistSize = (2 * c_lowOffset) / PARALLEL_BYTES;

  enum lzDecompressStates { WRITE_LITERAL, READ_MATCH, NO_OP };
  enum lzDecompressStates next_state =
      WRITE_LITERAL; // start from Read Literal Length

  ap_uint<c_parallelBit> ramHistory[2][c_ramHistSize];
#pragma HLS dependence variable = ramHistory inter false
#pragma HLS resource variable = ramHistory core = RAM_2P_URAM
#pragma HLS ARRAY_PARTITION variable = ramHistory dim = 1 complete

  ap_uint<c_parallelBit> regHistory[2][c_regHistSize];
// full partition  to infer as reg
#pragma HLS ARRAY_PARTITION variable = regHistory dim = 0 complete

  SIZE_DT lit_len = 0;
  SIZE_DT orig_lit_len = 0;
  uint32_t output_cnt = 0;
  uint16_t match_loc = 0;
  SIZE_DT match_len = 0;
  uint16_t write_idx = 0;
  uint16_t output_index = 0;
  uint32_t outSize = 0;

  bool outStreamFlag = false;

  ap_uint<16> offset = 0;
  ap_uint<c_parallelBit> outStreamValue = 0;
  ap_uint<2 * c_parallelBit> output_window;
  uint8_t parallelBits = 0;

  ap_uint<c_parallelBit> outValue = 0;
  bool matchDone = false;
  orig_lit_len = litlenStream.read();
  lit_len = orig_lit_len;
  output_cnt += lit_len;

  bool outFlag = false;
  uint8_t incr_output_index = 0;
lz4_decoder:
  for (; matchDone == false;) {
#pragma HLS PIPELINE II = 1
    bool veryLowOffsetFlag = false, matchLocFlag = false;
    uint16_t read_idx = match_loc / PARALLEL_BYTES;
    uint16_t byte_loc = (match_loc % PARALLEL_BYTES) % PARALLEL_BYTES;
    ap_uint<2 * c_parallelBit> localValue;
    ap_uint<c_parallelBit> lowValue, highValue;

    // always reading to make better timing
    ap_uint<c_parallelBit> lowValueReg =
        regHistory[0][(read_idx + 0) % c_regHistSize];
    ap_uint<c_parallelBit> highValueReg =
        regHistory[1][(read_idx + 1) % c_regHistSize];
    ap_uint<c_parallelBit> lowValueRam =
        ramHistory[0][(read_idx + 0) % c_ramHistSize];
    ap_uint<c_parallelBit> highValueRam =
        ramHistory[1][(read_idx + 1) % c_ramHistSize];

    if (offset < c_lowOffset) {
      lowValue = lowValueReg;
      highValue = highValueReg;
    } else {
      lowValue = lowValueRam;
      highValue = highValueRam;
    }

    localValue.range(c_parallelBit - 1, 0) = lowValue;
    localValue.range(2 * c_parallelBit - 1, c_parallelBit) = highValue;

    if (next_state == WRITE_LITERAL) {
      outFlag = true;
      bool writeLitState = true;
      // printf("WRITE_LITERAL\n");
      outValue = litStream.read();
      if (lit_len >= PARALLEL_BYTES) {
        incr_output_index = PARALLEL_BYTES;
        lit_len -= PARALLEL_BYTES;
        writeLitState = lit_len ? true : false;
      } else {
        incr_output_index = lit_len;
        lit_len = 0;
        writeLitState = false;
      }
      if (writeLitState) {
        next_state = WRITE_LITERAL;
      } else {
        offset = offsetStream.read();
        match_len = matchlenStream.read();
        match_loc = output_cnt - offset;
        output_cnt += match_len;
        if ((offset > 0) & (offset < c_veryLowOffset)) {
          parallelBits = 1;
          if (offset < PARALLEL_BYTES) {
            next_state = NO_OP;
          } else {
            next_state = READ_MATCH;
          }
        } else {
          parallelBits = PARALLEL_BYTES;
          next_state = READ_MATCH;
        }
      }
    } else if (next_state == READ_MATCH) {
      // printf("READ_MATCH\n");
      outFlag = true;
      outValue = localValue >> (byte_loc * 8);

      if (match_len >= parallelBits) {
        incr_output_index = parallelBits;
        match_loc += parallelBits;
        match_len -= parallelBits;
      } else {
        incr_output_index = match_len;
        match_loc += match_len;
        match_len = 0;
      }
      if (match_len) {
        next_state = READ_MATCH;
      } else {
        orig_lit_len = litlenStream.read();
        lit_len = orig_lit_len;
        output_cnt += orig_lit_len;
        if (orig_lit_len) {
          next_state = WRITE_LITERAL;
        } else {
          offset = offsetStream.read();
          match_len = matchlenStream.read();
          match_loc = output_cnt - offset;
          output_cnt += match_len;
          if ((offset > 0) & (offset < c_veryLowOffset)) {
            parallelBits = 1;
            if (offset < PARALLEL_BYTES) {
              next_state = NO_OP;
            } else {
              next_state = READ_MATCH;
            }
          } else {
            parallelBits = PARALLEL_BYTES;
            next_state = READ_MATCH;
          }
        }
      }
    } else if (next_state == NO_OP) {
      outFlag = false;
      // printf("NO_OP\n");
      // Adding NO_OP as workaround for low offset case as
      // for very low offset case, results are not matching
      next_state = READ_MATCH;
    } else {
      assert(0);
    }

    if (orig_lit_len == 0 && match_len == 0) {
      matchDone = true;
    }

    if (outFlag) {
      output_window.range((output_index + PARALLEL_BYTES) * 8 - 1,
                          output_index * 8) = outValue;
      output_index += incr_output_index;
    }
    uint8_t localOutputIdx = output_index - PARALLEL_BYTES;
    bool outputIdxFlag = ((output_index >= PARALLEL_BYTES));

    outStreamValue = output_window.range(c_parallelBit - 1, 0);
    regHistory[0][write_idx % c_regHistSize] = outStreamValue;
    regHistory[1][write_idx % c_regHistSize] = outStreamValue;
    ramHistory[0][write_idx % c_ramHistSize] = outStreamValue;
    ramHistory[1][write_idx % c_ramHistSize] = outStreamValue;

    bool outStreamFlag = false;
    if (outputIdxFlag) {
      write_idx++;
      output_window >>= PARALLEL_BYTES * 8;
      output_index = localOutputIdx;
      outStreamFlag = true;
    }

    if (outStreamFlag) {
      outStream << outStreamValue;
      endOfStream << 0;
      outSize += PARALLEL_BYTES;
    }
  }

  // output_index:%d\n",lit_len,match_len,incr_output_index,output_index);
  // Write out if there is remaining left over data in output buffer
  // to outStream
  if (output_index) {
    outStreamValue = output_window.range(c_parallelBit - 1, 0);
    outStream << outStreamValue;
    endOfStream << 0;
    outSize += output_index;
  }
  outStream << 0;
  endOfStream << 1;
  sizeOutStream << outSize;
}

template <int HISTORY_SIZE, int LOW_OFFSET = 8>
void lzDecompressZlibEos(hls::stream<ap_uint<32>> &inStream,
                         hls::stream<bool> &inStream_eos,
                         hls::stream<ap_uint<8>> &outStream,
                         hls::stream<bool> &outStream_eos,
                         hls::stream<uint64_t> &outSize_val) {
  enum lz_d_states { READ_STATE, MATCH_STATE, LOW_OFFSET_STATE };
  uint8_t local_buf[HISTORY_SIZE];
#pragma HLS dependence variable = local_buf inter false

  uint32_t match_len = 0;
  uint32_t out_len = 0;
  uint32_t match_loc = 0;
  uint32_t length_extract = 0;
  lz_d_states next_states = READ_STATE;
  uint16_t offset = 0;
  ap_uint<32> nextValue, currValue;
  ap_uint<8> outValue = 0;
  ap_uint<8> prevValue[LOW_OFFSET];
#pragma HLS ARRAY_PARTITION variable = prevValue dim = 0 complete
  uint64_t out_cntr = 0;

  bool eos_flag = inStream_eos.read();
  nextValue = inStream.read();
lz_decompress:
  for (uint32_t i = 0; (eos_flag == false) || (next_states != READ_STATE);
       i++) {
#pragma HLS PIPELINE II = 1
    ////printme("Start of the loop %d state %d \n", i, next_states);
    if (next_states == READ_STATE) {
      currValue = nextValue;
      eos_flag = inStream_eos.read();
      nextValue = inStream.read();
      offset = currValue.range(15, 0);
      length_extract = currValue.range(31, 16);
      // printme("offset %d length_extract %d \n", offset, length_extract);
      if (length_extract) {
        match_loc = i - offset;
        match_len = length_extract;
        ////printme("HISTORY=%x\n",(uint8_t)outValue);
        out_len = match_len - 1;
        if (offset >= LOW_OFFSET) {
          next_states = MATCH_STATE;
          outValue = local_buf[match_loc % HISTORY_SIZE];
        } else {
          next_states = LOW_OFFSET_STATE;
          outValue = prevValue[offset - 1];
        }
        match_loc++;
      } else {
        outValue = currValue.range(7, 0);
      }
    } else if (next_states == LOW_OFFSET_STATE) {
      outValue = prevValue[offset - 1];
      match_loc++;
      if (out_len == 1) {
        next_states = READ_STATE;
      }
      if (out_len) {
        out_len--;
      }
    } else {
      outValue = local_buf[match_loc % HISTORY_SIZE];
      ////printme("HISTORY=%x\n",(uint8_t)outValue);
      match_loc++;
      if (out_len == 1) {
        next_states = READ_STATE;
      }
      if (out_len) {
        out_len--;
      }
    }
    local_buf[i % HISTORY_SIZE] = outValue;

    outStream << outValue;
    out_cntr++;
    outStream_eos << 0;

    // printme("%c", (uint8_t)outValue);
    for (uint32_t pIdx = LOW_OFFSET - 1; pIdx > 0; pIdx--) {
#pragma HLS UNROLL
      prevValue[pIdx] = prevValue[pIdx - 1];
    }
    prevValue[0] = outValue;
  }
  // printme("Exited main for-llop \n");

  outStream << 0;
  outStream_eos << 1;

  outSize_val << out_cntr;
}

template <int HISTORY_SIZE, int LOW_OFFSET = 8>
void lzDecompressZlibEosStream(hls::stream<ap_uint<32>> &inStream,
                               hls::stream<bool> &inStream_eos,
                               hls::stream<ap_uint<8>> &outStream,
                               hls::stream<bool> &outStream_eos,
                               hls::stream<uint32_t> &outSize_val,
                               hls::stream<uint32_t> &inSize) {
  enum lz_d_states { READ_STATE, MATCH_STATE, LOW_OFFSET_STATE };
  for (uint32_t input_size = inSize.read(); input_size != 0;
       input_size = inSize.read()) {
    uint8_t local_buf[HISTORY_SIZE];
#pragma HLS dependence variable = local_buf inter false

    uint32_t match_len = 0;
    uint32_t out_len = 0;
    uint32_t match_loc = 0;
    uint32_t length_extract = 0;
    lz_d_states next_states = READ_STATE;
    uint16_t offset = 0;
    ap_uint<32> nextValue, currValue;
    ap_uint<8> outValue = 0;
    ap_uint<8> prevValue[LOW_OFFSET];
#pragma HLS ARRAY_PARTITION variable = prevValue dim = 0 complete
    uint32_t out_cntr = 0;

    bool eos_flag = inStream_eos.read();
    nextValue = inStream.read();
  lz_decompress:
    for (uint32_t i = 0; (eos_flag == false) || (next_states != READ_STATE);
         i++) {
#pragma HLS PIPELINE II = 1
      ////printme("Start of the loop %d state %d \n", i, next_states);
      if (next_states == READ_STATE) {
        currValue = nextValue;
        eos_flag = inStream_eos.read();
        nextValue = inStream.read();
        offset = currValue.range(15, 0);
        length_extract = currValue.range(31, 16);
        // printme("offset %d length_extract %d \n", offset, length_extract);
        if (length_extract) {
          match_loc = i - offset;
          match_len = length_extract;
          ////printme("HISTORY=%x\n",(uint8_t)outValue);
          out_len = match_len - 1;
          if (offset >= LOW_OFFSET) {
            next_states = MATCH_STATE;
            outValue = local_buf[match_loc % HISTORY_SIZE];
          } else {
            next_states = LOW_OFFSET_STATE;
            outValue = prevValue[offset - 1];
          }
          match_loc++;
        } else {
          outValue = currValue.range(7, 0);
        }
      } else if (next_states == LOW_OFFSET_STATE) {
        outValue = prevValue[offset - 1];
        match_loc++;
        if (out_len == 1) {
          next_states = READ_STATE;
        }
        if (out_len) {
          out_len--;
        }
      } else {
        outValue = local_buf[match_loc % HISTORY_SIZE];
        ////printme("HISTORY=%x\n",(uint8_t)outValue);
        match_loc++;
        if (out_len == 1) {
          next_states = READ_STATE;
        }
        if (out_len) {
          out_len--;
        }
      }
      local_buf[i % HISTORY_SIZE] = outValue;

      outStream << outValue;
      out_cntr++;
      outStream_eos << 0;

      // printme("%c", (uint8_t)outValue);
      for (uint32_t pIdx = LOW_OFFSET - 1; pIdx > 0; pIdx--) {
#pragma HLS UNROLL
        prevValue[pIdx] = prevValue[pIdx - 1];
      }
      prevValue[0] = outValue;
    }
    // printme("Exited main for-llop \n");

    outStream << 0;
    outStream_eos << 1;

    outSize_val << out_cntr;
  }
  outSize_val << 0;
}

} // namespace compression
} // namespace xf
#endif // _XFCOMPRESSION_LZ_DECOMPRESS_HPP_
