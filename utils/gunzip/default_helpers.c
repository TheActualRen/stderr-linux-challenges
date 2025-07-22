#include "default_helpers.h"
#include <stdint.h>

/*
	- These were just some functions needed for deflate but were quite long in their own regard,
	- so I seperated them

	What these functions do:
	- DEFLATE doesn't directly store lengths and distances
	- Instead, lengths (3-258) are encoded using symbols 257-285
	- Distances (1-32768) use symbols 0-29
	- Many of these symbols need extra bits to specify exact values

	- These helper functions map a raw length/distance value to 
		- appropriate DEFLATE symbol (a code)
		- the number of extra bits required
		- and the extra bits' value
*/

static const uint16_t length_base[29] = {
	3, 4, 5, 6, 7, 8, 9, 10,
    11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115,
    131, 163, 195, 227, 258
};

static const uint8_t length_extra_bits[29] = {
	0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4,
    5, 5, 5, 5, 0
};

void length_to_code(uint16_t length, uint16_t* code, uint8_t* extra_bits,
                    uint16_t* extra_value) {

	if (length < 3) length = 3;
	if (length > 258) length = 258;
	
	for (int i = 0; i < 29; i++) {
		uint16_t base = length_base[i];
		uint8_t bits = length_extra_bits[i];
		uint16_t next_base = (i < 28) ? length_base[i + 1] : 259;

		if (length >= base && length < next_base) {
			*code = 257 + i;
			*extra_bits = bits;
			*extra_value = length - base;
			return;
		}
	}
	*code = 285;
	*extra_bits = 0;
	*extra_value = 0;
}

static const uint16_t dist_base[30] = {
    1, 2, 3, 4, 5, 7, 9, 13,
    17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073,
    4097, 6145, 8193, 12289, 16385, 24577
};

static const uint8_t dist_extra_bits[30] = {
    0, 0, 0, 0, 1, 1, 2, 2,
    3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10,
    11, 11, 12, 12, 13, 13
};

void distance_to_code(uint16_t distance, uint16_t* code, uint8_t* extra_bits,
                      uint16_t* extra_value) {

	if (distance == 0) { 
		distance = 1;
	}

	if (distance > 32768) {
		distance = 32768;
	}

	for (int i = 0; i < 30; i++) {
		uint16_t base = dist_base[i];
		uint8_t bits = dist_extra_bits[i];
		uint16_t next_base = (i < 29) ? dist_base[i + 1] : 32769;

		if (distance >= base && distance < next_base) {
			*code = i;
			*extra_bits = bits;
			*extra_value = distance - base;
			return;
		}
	}

	*code = 29;
	*extra_bits = 0;
	*extra_value = 0;
}


