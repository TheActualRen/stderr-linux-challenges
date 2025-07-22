#include "bitstream.h"

#include <stdint.h>
#include <string.h>

/*
	- DEFLATE algo and gzip work with streams of bits, not just bytes
	- We must pack the variable-length codes bit-by-bit efficiently

	What the code does
	- Maintains a bit buffer and current position
	- Allows writing n bits a time
	- Buffers bits until a full byte can be output
	- Supports flushing remaining bits at the end

*/

void bitwriter_init(BitWriter* bw, uint8_t* buf, size_t capacity) {
	bw->buf = buf;
	bw->capacity = capacity;
	bw->byte_pos = 0;
	bw->bit_pos = 0;
	memset(buf, 0, capacity);
}

void bitwriter_write_bits(BitWriter* bw, uint16_t bits, uint8_t bitlen) {
	for (uint8_t i = 0; i < bitlen; i++) {
		if (bw->byte_pos >= bw->capacity) {
			return;
		}

		uint8_t bit = (bits >> i) & 1;
		bw->buf[bw->byte_pos] |= bit << bw->bit_pos;

		bw->bit_pos++;
		if (bw->bit_pos == 8) {
			bw->bit_pos = 0;
			bw->byte_pos++;

			if (bw->byte_pos < bw->capacity) {
				bw->buf[bw->byte_pos] = 0;
			}
		}
	}
}

void bitwriter_flush(BitWriter* bw) {
	if (bw->bit_pos > 0) {
		bw->byte_pos++;
		bw->bit_pos = 0;
	}
}

size_t bitwriter_bytes_written(BitWriter* bw) {
    return bw->byte_pos + (bw->bit_pos > 0 ? 1 : 0);
}
