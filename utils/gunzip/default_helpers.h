#ifndef DEFLATE_HELPERS_H
#define DEFLATE_HELPERS_H

#include <stdint.h>

void length_to_code(uint16_t length, uint16_t* code, uint8_t* extra_bits,
                    uint16_t* extra_value);

void distance_to_code(uint16_t distance, uint16_t* code, uint8_t* extra_bits,
                      uint16_t* extra_value);

#endif
