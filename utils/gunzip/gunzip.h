#ifndef GUNZIP_H
#define GUNZIP_H

#include "deflate.h"

void gunzip_encode(const char* input_path, const char* output_path, DeflateBlockType block_type);

#endif

