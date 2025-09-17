#ifndef HUFFMAN_DYNAMIC_H
#define HUFFMAN_DYNAMIC_H

#include <stdint.h>
#include <stddef.h>
		
typedef struct {
	uint16_t code;
	uint8_t bitlen;
} HuffmanCodeDynamic;

typedef struct HuffmanNode HuffmanNode;


HuffmanNode* build_huffman_tree(const uint32_t* freqs, size_t num_symbols);
void generate_code_lengths(HuffmanNode* node, uint8_t depth, uint8_t* bitlens);
void limit_code_lengths(uint8_t* bitlens, size_t num_symbols, uint8_t max_bits);

void generate_canonical_codes(const uint8_t* bitlens, size_t num_symbols,
							  HuffmanCodeDynamic* out_codes); 

void free_huffman_tree(HuffmanNode* node);

#endif
