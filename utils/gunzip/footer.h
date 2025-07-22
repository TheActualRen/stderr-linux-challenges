#ifndef FOOTER_H
#define FOOTER_H

#include <stdint.h>
#include <stdio.h>

uint32_t crc32(uint32_t crc, const uint8_t* buf, size_t len);

void write_gzip_footer(FILE* out, uint32_t crc, uint32_t isize);

#endif
