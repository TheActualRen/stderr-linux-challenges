#ifndef HUFFMAN_FIXED_H
#define HUFFMAN_FIXED_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint16_t code;
	uint8_t bitlen;
} HuffmanCode;

void init_fixed_huffman_tables(void);
HuffmanCode get_fixed_literal_code(uint16_t symbol);
HuffmanCode get_fixed_distance_code(uint16_t dist);

#endif
