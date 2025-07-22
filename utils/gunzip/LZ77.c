#include "LZ77.h"

#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 32768
#define MAX_MATCH 258
#define MIN_MATCH 3

/*
	- LZ77 compresses data by replacing repeated strings with references to previous occurences	
	- It maintains a sliding window over the input
	- It finds the longest match of the lookahead buffer in the sliding window
	- If match length >= minimum (which is 3), output a (distance, length) pair
	- Otherwise, output a literal byte
	
	What the code does
	- We have a sliding window of size 32KB (32768 bytes)
	- We set max length to 258 bytes
	- min length to 3 bytes (any shorter and I think it can become a longer input)
	- The code searches backwards in the window to find the longest match
	- Produces a list of tokens representing literals or matches
	- The token list can be freed later to free up memory

	Improvements
	- currently this is a brute-force approach
	- could implement some sort of hash table or trie/tree to improve performance
*/

void lz77_free_tokens(LZ77TokenList* list) {
	if (list && list->tokens) {
		free(list->tokens);
		list->tokens = NULL;
		list->count = 0;
		list->capacity = 0;
	}
}

static void append_token(LZ77TokenList* list, LZ77Token token) {
	if (list->count >= list->capacity) {
		list->capacity = list->capacity ? list->capacity * 2 : 128;
		list->tokens = realloc(list->tokens, list->capacity * sizeof(LZ77Token));
	}
	list->tokens[list->count++] = token;
}

void lz77_compress(const uint8_t* input, size_t input_len, LZ77TokenList* out_tokens) {
	memset(out_tokens, 0, sizeof(*out_tokens));

	for (size_t pos = 0; pos < input_len;) {
		size_t best_length = 0;
		size_t best_distance = 0;

		size_t start = (pos > WINDOW_SIZE) ? pos - WINDOW_SIZE : 0;

		for (size_t search = start; search < pos; search++) {
			size_t match_len = 0;

			while (match_len < MAX_MATCH && 
				   pos + match_len < input_len && 
				   input[search + match_len] == input[pos + match_len]) {
				match_len++;
			}

			if (match_len >= MIN_MATCH && match_len > best_length) {
				best_length = match_len;
				best_distance = pos - search;
				if (match_len == MAX_MATCH) break;
			}
		}

		if (best_length >= MIN_MATCH) {
			LZ77Token token = {
				.type = LZ77_MATCH,
				.match = { .length = best_length, .distance = best_distance }
			};
			append_token(out_tokens, token);
			pos += best_length;
		} else {
			LZ77Token token = {
				.type = LZ77_LITERAL,
				.literal = input[pos]
			};
			append_token(out_tokens, token);
			pos += 1;
		}
	}
}
