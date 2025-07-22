#include "gunzip.h"
#include "header.h"
#include "footer.h"
#include "LZ77.h"
#include "deflate.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 65536
#define COMPRESSED_BUFFER_SIZE (MAX_INPUT_SIZE + 100)

/*
	What the code does:
	- Abstracts the compression workflow so when we need to call gunzip, we just need 2 lines
	- It reads the input file entirely into memory
	- Compresses using specified DEFLATE block type (need to add dynamic)
	- Writes gzip header, compressed datam and the gzip footer
	- Handles error checking in file operations
*/

void gunzip_encode(const char* input_path, const char* output_path, 
                   DeflateBlockType block_type) {

	FILE* fin = fopen(input_path, "rb");
	if (!fin) { 
		perror("Failed to open input file");
		exit(1);
	}

	uint8_t input_buf[MAX_INPUT_SIZE];
	size_t input_len = fread(input_buf, 1, MAX_INPUT_SIZE, fin);
	fclose(fin);

	if (input_len == 0) {
		fprintf(stderr, "Input file is empty\n");
		exit(1);
	}

	uint8_t compressed[COMPRESSED_BUFFER_SIZE];
	size_t deflate_size = 0;

	if (block_type == DEFLATE_BLOCK_FIXED) {
		LZ77TokenList tokens;
		lz77_compress(input_buf, input_len, &tokens);

		deflate_size = deflate_encode_block(
			compressed, COMPRESSED_BUFFER_SIZE,
			block_type,
			NULL, 0,
			tokens.tokens, tokens.count,
			1 // final block
		);

		lz77_free_tokens(&tokens);

	} else if (block_type == DEFLATE_BLOCK_STORED) {
		deflate_size = deflate_encode_block(
			compressed, COMPRESSED_BUFFER_SIZE,
			block_type,
			input_buf, input_len,
			NULL, 0,
			1 // final block
		);
	} else {
		fprintf(stderr, "Unsupported block type\n");
		exit(1);
	}

	FILE* fout = fopen(output_path, "wb");
	if (!fout) {
		perror("Failed to open output file");
		exit(1);
	}

	write_gzip_header(fout);
	fwrite(compressed, 1, deflate_size, fout);

	uint32_t crc = crc32(0, input_buf, input_len);
	write_gzip_footer(fout, crc, input_len);

	fclose(fout);
}

