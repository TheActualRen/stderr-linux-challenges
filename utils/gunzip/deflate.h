#ifndef DEFLATE_H
#define DEFLATE_H

#include "LZ77.h"

#include <stddef.h>
#include <stdint.h>

typedef enum {
  DEFLATE_BLOCK_STORED = 0,
  DEFLATE_BLOCK_FIXED = 1,
  DEFLATE_BLOCK_DYNAMIC = 2
} DeflateBlockType;

size_t deflate_encode_block(uint8_t* out_buf, size_t out_buf_size,
                            DeflateBlockType block_type, const uint8_t* data,
                            size_t data_len, const LZ77Token* tokens,
                            size_t token_count, int is_final_block);

#endif
