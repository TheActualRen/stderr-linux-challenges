#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint8_t* buf;
	size_t capacity;
	size_t byte_pos;
	uint8_t bit_pos;
} BitWriter;

void bitwriter_init(BitWriter* bw, uint8_t* buf, size_t capacity);
void bitwriter_write_bits(BitWriter* bw, uint16_t bits, uint8_t bitlen);
void bitwriter_flush(BitWriter* bw);
size_t bitwriter_bytes_written(BitWriter* bw);

#endif
