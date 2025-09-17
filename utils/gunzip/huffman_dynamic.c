#include "huffman_dynamic.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_SYMBOLS 1024
#define MAX_BITLEN 15

typedef struct HuffmanNode {
	uint16_t symbol;
	uint32_t freq;
	struct HuffmanNode* left;
	struct HuffmanNode* right;
} HuffmanNode;

typedef struct {
	HuffmanNode* items[MAX_SYMBOLS];
	int size;
} PriorityQueue;

static void swap(HuffmanNode** a, HuffmanNode** b) {
	HuffmanNode* temp = *a;
	*a = *b;
	*b = temp;
}

static void heapify_up(PriorityQueue* pq, int idx) {
	while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (pq->items[parent]->freq <= pq->items[idx]->freq) break;
        swap(&pq->items[parent], &pq->items[idx]);
        idx = parent;
    }
}

static void heapify_down(PriorityQueue* pq, int idx) {
	int smallest = idx;
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        smallest = idx;

        if (left < pq->size && pq->items[left]->freq < pq->items[smallest]->freq)
            smallest = left;
        if (right < pq->size && pq->items[right]->freq < pq->items[smallest]->freq)
            smallest = right;
        if (smallest == idx) break;

        swap(&pq->items[idx], &pq->items[smallest]);
        idx = smallest;
    }
}
	
static void enqueue(PriorityQueue* pq, HuffmanNode* newNode) {
	if (pq->size == MAX_SYMBOLS) {
		fprintf(stderr, "Heap is FULL\n");
		return;
	}
	pq->items[pq->size] = newNode;
	heapify_up(pq, pq->size);
	pq->size++;
}

static HuffmanNode* dequeue(PriorityQueue* pq) {
	if (!pq->size) {
		fprintf(stderr, "Heap is EMPTY\n");
		return NULL;
	}

	HuffmanNode* node = pq->items[0];
	pq->items[0] = pq->items[--pq->size];
	pq->items[pq->size] = NULL;
	heapify_down(pq, 0);

	return node;
}

static HuffmanNode* create_node(uint32_t symbol, uint32_t freq) {
	HuffmanNode* newNode = malloc(sizeof(HuffmanNode));
	newNode->symbol = symbol;
	newNode->freq = freq;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

HuffmanNode* build_huffman_tree(const uint32_t* freqs, size_t num_symbols) {
	PriorityQueue pq = { .size = 0 };

	for (size_t i = 0; i < num_symbols; i++) {
		if (freqs[i] > 0) {
			enqueue(&pq, create_node(i, freqs[i]));
		}
	}

	if (pq.size == 0) return NULL;

	if (pq.size == 1) {
		HuffmanNode* only = dequeue(&pq);

		uint16_t dummy_symbol = (only->symbol == 0) ? 1 : 0;
		HuffmanNode* dummy = create_node(dummy_symbol, 0);

		HuffmanNode* parent = create_node(0, only->freq);

		parent->left = only;
		parent->right = dummy;

		return parent;
	}

	while (pq.size > 1) {
		HuffmanNode* left = dequeue(&pq);
		HuffmanNode* right = dequeue(&pq);
		HuffmanNode* parent = create_node(0, left->freq + right->freq);

		parent->left = left;
		parent->right = right;

		enqueue(&pq, parent);
	}

	return dequeue(&pq);
}

void generate_code_lengths(HuffmanNode* node, uint8_t depth, uint8_t* bitlens) {
	if (!node) return;

	if (!node->left && !node->right) {
		if (depth > MAX_BITLEN) depth = MAX_BITLEN;
		bitlens[node->symbol] = depth;
		return;
	}

	generate_code_lengths(node->left, depth + 1, bitlens);
	generate_code_lengths(node->right, depth + 1, bitlens);
}

void limit_code_lengths(uint8_t* bitlens, size_t num_symbols, uint8_t max_bits) {
    int bl_count[32] = {0};

    for (size_t i = 0; i < num_symbols; i++) {
        if (bitlens[i] > 0) {
            if (bitlens[i] > max_bits) bitlens[i] = max_bits;
            bl_count[bitlens[i]]++;
        }
    }

    int total = 0;

    for (int i = 1; i <= max_bits; i++) {
        total = (total << 1) + bl_count[i];
    }

    while (total > (1 << max_bits)) {
        for (int i = max_bits; i > 0; i--) {
            if (bl_count[i]) {
                bl_count[i]--;
                bl_count[i - 1] += 2;
                total--;
                break;
            }
        }
    }

    size_t reassigned = 0;

    for (int len = max_bits; len > 0; len--) {
        for (size_t i = 0; i < num_symbols; i++) {
            if (bitlens[i] == len && bl_count[len] == 0) {
                for (int newLen = len - 1; newLen > 0; newLen--) {
                    if (bl_count[newLen]) {
                        bitlens[i] = newLen;
                        bl_count[newLen]--;
                        reassigned++;
                        break;
                    }
                }
            } else if (bitlens[i] == len) {
                bl_count[len]--;
            }
        }
    }
}

/*void limit_code_lengths(uint8_t* bitlens, size_t num_symbols, uint8_t max_bits) {*/
/*	int bl_count[32] = {0};*/
/**/
/*	for (size_t i = 0; i < num_symbols; i++) {*/
/*		if (bitlens[i] > 0) {*/
/*			if (bitlens[i] > max_bits) bitlens[i] = max_bits;*/
/*			bl_count[bitlens[i]]++;*/
/*		}*/
/*	}*/
/**/
/*	int total = 0;*/
/**/
/*	for (int i = 1; i <= max_bits; i++) {*/
/*		total = (total << 1) + bl_count[i];*/
/*	}*/
/**/
/*	while (total > (1 << max_bits)) {*/
/*		for (int i = max_bits; i > 0; i--) {*/
/*			if (bl_count[i]) {*/
/*				bl_count[i]--;*/
/*				bl_count[i - 1] += 2;*/
/*				total--;*/
/*				break;*/
/*			}*/
/*		}*/
/*	}*/
/**/
/*	size_t reassigned = 0;*/
/**/
/*	for (int len = max_bits; len > 0; len--) {*/
/*		for (size_t i = 0; i < num_symbols; i++) {*/
/*			if (bitlens[i] == len && bl_count[len] == 0) {*/
/*				for (int newLen = len - 1; newLen > 0; newLen--) {*/
/*					if (bl_count[newLen]) {*/
/*						bitlens[i] = newLen;*/
/*						bl_count[newLen]--;*/
/*						reassigned++;*/
/*						break;*/
/*					}*/
/*				}*/
/*			} else if (bitlens[i] == len) {*/
/*				bl_count[len]--;*/
/*			}*/
/*		}*/
/*	}*/
/*}*/

void generate_canonical_codes(const uint8_t* bitlens, size_t num_symbols,
							  HuffmanCodeDynamic* out_codes) {

	uint16_t bl_count[16] = {0};
	uint16_t next_code[16] = {0};

	for (size_t i = 0; i < num_symbols; i++) {
		if (bitlens[i] > 0) {
			bl_count[bitlens[i]]++;
		}
	}

	uint16_t code = 0;

	for (int bits = 1; bits <= MAX_BITLEN; bits++) {
		code = (code + bl_count[bits - 1]) << 1;
		next_code[bits] = code;
	}

	for (size_t i = 0; i < num_symbols; i++) {
		uint8_t len = bitlens[i];

		if (len > 0) {
			out_codes[i].bitlen = len;
			out_codes[i].code = next_code[len]++;
		} else {
			out_codes[i].bitlen = 0;
			out_codes[i].code = 0;
		}
	}
}

void free_huffman_tree(HuffmanNode* node) {
	if (!node) return;
	free_huffman_tree(node->left);
	free_huffman_tree(node->right);
	free(node);
}
