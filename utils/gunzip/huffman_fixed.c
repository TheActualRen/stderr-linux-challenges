#include "huffman_fixed.h"

/*
	- The DEFLATE algo uses Huffmnan coding to encode literals/lengths and distances
	- It supports both dynamic HUffman codes and fixed (this being the fixed one)
	- Fix codes are predefined and standardised (RFC 1951)

	What the code does:
	- we generate two look up tables:
		- literal_table: maps literal byte values and length symbols to fixed Huffman Codes
		- distance_table: maps distances to fixed Huffman codes
	- Codes are stored with bit length and bit-reversed codeword
		- we reverse because DEFLATE sends bits from least significant first
	- The reverse bits function flips bits of the code to match this 
*/

static HuffmanCode literal_table[286];
static HuffmanCode distance_table[32];

static uint16_t reverse_bits(uint16_t code, uint8_t bitlen) {
	uint16_t reversed = 0;

	for (uint8_t i = 0; i < bitlen; i++) {
		reversed = (reversed << 1) | (code & 1);
		code >>= 1;
	}
	return reversed;
}

void init_fixed_huffman_tables(void) {
	for (uint16_t i = 0; i <= 143; i++) {
		literal_table[i].bitlen = 8;
		literal_table[i].code = reverse_bits(0x30 + i, 8);
	}

	for (uint16_t i = 144; i <= 255; i++) {
		literal_table[i].bitlen = 9;
		literal_table[i].code = reverse_bits(0x190 + (i - 144), 9);
	}

	for (uint16_t i = 256; i <= 279; i++) {
		literal_table[i].bitlen = 7;
		literal_table[i].code = reverse_bits(i - 256, 7);
	}

	for (uint16_t i = 280; i <= 285; i++) {
		literal_table[i].bitlen = 8;
		literal_table[i].code = reverse_bits(0xC0 + (i - 280), 8);
	}

	for (uint16_t i = 0; i < 32; i++) {
		distance_table[i].bitlen = 5;
		distance_table[i].code = reverse_bits(i, 5);
	}
}

HuffmanCode get_fixed_literal_code(uint16_t symbol) {
	return literal_table[symbol];
}

HuffmanCode get_fixed_distance_code(uint16_t dist){
	return distance_table[dist];
}
