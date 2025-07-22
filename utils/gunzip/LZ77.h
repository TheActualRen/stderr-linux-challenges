#ifndef LZ77_H
#define LZ77_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
	LZ77_LITERAL,
	LZ77_MATCH
} LZ77TokenType;

typedef struct {
	LZ77TokenType type;

	union {
		uint8_t literal;

		struct {
			uint16_t length;
			uint16_t distance;
		} match;
	};
} LZ77Token;

typedef struct {
	LZ77Token* tokens;
	size_t count;
	size_t capacity;
} LZ77TokenList;

void lz77_compress(const uint8_t* input, size_t input_len, LZ77TokenList* out_tokens);
void lz77_free_tokens(LZ77TokenList* list);

#endif
