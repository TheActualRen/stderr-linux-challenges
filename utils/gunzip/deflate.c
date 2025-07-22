#include "deflate.h"
#include "bitstream.h"
#include "huffman_fixed.h"
#include "default_helpers.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
	THIS IS THE MEAT AND POTATOES

	What is DEFLATE
	- DEFLATE is an encoding algo that combines LZ77 Compression with Huffman coding
	- There are three block types that the DEFLATE algo can use
		- Stored (uncompressed) blocks
			- since some data is actually shorter left uncompressed
		- Fixed Huffman blocks (uses fixed code tables)
		- Dynamic Huffman blocks (builds Huffman codes from data) (NEED TO DO THIS ONE)
	
	What the code does
	- Accepts block, input data or LZ77 tokens
	- For stored blocks, we output raw data with length headers
	- For fixed Huffman blocks, we encode tokens using Huffman tables
	- Writes the block header with the final-block flag
	- Uses bitstream writer to output bits to the compressed buffer

*/

static size_t write_stored_block(uint8_t* out_buf, size_t out_buf_size,
                                 const uint8_t* data, size_t data_len,
                                 int is_final_block) {

	if (out_buf_size < data_len + 5) return 0;

	out_buf[0] = (uint8_t)(is_final_block & 1);
	size_t pos = 1;

	uint16_t len = (uint16_t)data_len;
	uint16_t nlen = ~len;

	out_buf[pos++] = (uint8_t)(len & 0xFF);
	out_buf[pos++] = (uint8_t)(len >> 8);
	out_buf[pos++] = (uint8_t)(nlen & 0xFF);
	out_buf[pos++] = (uint8_t)(nlen >> 8);

	memcpy(out_buf + pos, data, data_len);
	pos += data_len;

	return pos;
}

static size_t write_fixed_block(uint8_t* out_buf, size_t out_buf_size,
                                const LZ77Token* tokens, size_t token_count,
                                int is_final_block) {

	BitWriter bw;
	bitwriter_init(&bw, out_buf, out_buf_size);

	uint8_t header = (is_final_block & 1) | (1 << 1);
	bitwriter_write_bits(&bw, header, 3);

	init_fixed_huffman_tables();

	for (size_t i = 0; i < token_count; i++) {
		if (tokens[i].type == LZ77_LITERAL) {
			  HuffmanCode hcode = get_fixed_literal_code(tokens[i].literal);
			  bitwriter_write_bits(&bw, hcode.code, hcode.bitlen);
		} else if (tokens[i].type == LZ77_MATCH) {
			uint16_t length = tokens[i].match.length;
			uint16_t distance = tokens[i].match.distance;

			uint16_t length_code;
			uint8_t length_extra_bits;
			uint16_t length_extra_value;

			length_to_code(length, &length_code, &length_extra_bits,
						 &length_extra_value);
		  
			HuffmanCode length_huff = get_fixed_literal_code(length_code);
			bitwriter_write_bits(&bw, length_huff.code, length_huff.bitlen);
			if (length_extra_bits > 0) {
				bitwriter_write_bits(&bw, length_extra_value, length_extra_bits);
			}

			uint16_t dist_code;
			uint8_t dist_extra_bits;
			uint16_t dist_extra_value;

            distance_to_code(distance, &dist_code, &dist_extra_bits,
							 &dist_extra_value);

			HuffmanCode dist_huff = get_fixed_distance_code(dist_code);
			bitwriter_write_bits(&bw, dist_huff.code, dist_huff.bitlen);
			if (dist_extra_bits > 0) {
				bitwriter_write_bits(&bw, dist_extra_value, dist_extra_bits);
			}
		}
	}

	HuffmanCode eob = get_fixed_literal_code(256);
	bitwriter_write_bits(&bw, eob.code, eob.bitlen);
	bitwriter_flush(&bw);

	return bitwriter_bytes_written(&bw);
}

size_t deflate_encode_block(uint8_t* out_buf, size_t out_buf_size,
                            DeflateBlockType block_type, const uint8_t* data,
                            size_t data_len, const LZ77Token* tokens,
                            size_t token_count, int is_final_block) {
	
	switch (block_type) {
		case DEFLATE_BLOCK_STORED:
			return write_stored_block(out_buf, out_buf_size, data, data_len,
								      is_final_block);

		case DEFLATE_BLOCK_FIXED:
			return write_fixed_block(out_buf, out_buf_size, tokens, token_count,
									 is_final_block);

		case DEFLATE_BLOCK_DYNAMIC:
			return 0;

		default:
			return 0;
	}
}


