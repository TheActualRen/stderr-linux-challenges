#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 512

// TODO:
// 1) Populate a Huffman Tree so that I can get the coded bits for a character
//		- Need to build a BINARY_TREE
//		- So I need to implement a PRIORITY_QUEUE
//		- For the PRIORITY_QUEUE will need a linked list struct and queue struct
//		- For the BINARY_TREE I will need a tree node struct

typedef struct HuffmanNode {
	char c;
	int freq;
	struct HuffmanNode* left;
	struct HuffmanNode* right;
} HuffmanNode;

typedef struct {
	HuffmanNode* items[MAX];
	int size;
} MinHeap;

typedef struct QueueNode {
	HuffmanNode* data;
	struct QueueNode* next;
} QueueNode;

void swap(HuffmanNode** a, HuffmanNode** b) {
	HuffmanNode* temp = *a;
	*a = *b;
	*b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
	if (idx == 0) {
		return;
	}

	const unsigned int parentNodeIdx = (idx - 1) / 2;

	if (heap->items[parentNodeIdx]->freq > heap->items[idx]->freq) {
		swap(&heap->items[parentNodeIdx], &heap->items[idx]);
		heapify_up(heap, parentNodeIdx);
	}
}

void heapify_down(MinHeap* heap, int idx) {
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < heap->size &&
	    heap->items[left]->freq < heap->items[smallest]->freq) {
		smallest = left;
	}

	if (right < heap->size &&
	    heap->items[right]->freq < heap->items[smallest]->freq) {
		smallest = right;
	}

	if (smallest != idx) {
		swap(&heap->items[idx], &heap->items[smallest]);
		heapify_down(heap, smallest);
	}
}

void enqueue(MinHeap* heap, HuffmanNode* newNode) {
	if (heap->size == MAX) {
		fprintf(stderr, "Heap is full\n");
		return;
	}
	heap->items[heap->size] = newNode;
	heapify_up(heap, heap->size);
	heap->size++;
}

HuffmanNode* dequeue(MinHeap* heap) {
	if (!heap->size) {
		fprintf(stderr, "Heap Empty\n");
		return NULL;
	}

	HuffmanNode* node = heap->items[0];
	heap->items[0] = heap->items[--heap->size];
	heap->items[heap->size] = NULL;
	heapify_down(heap, 0);

	return node;
}

HuffmanNode* create_node(char c, int freq) {
	HuffmanNode* newNode = malloc(sizeof(HuffmanNode));
	newNode->c = c;
	newNode->freq = freq;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

int main() {
	MinHeap heap = {.size = 0};

	enqueue(&heap, create_node('a', 3));
	enqueue(&heap, create_node('b', 5));
	enqueue(&heap, create_node('c', 6));
	enqueue(&heap, create_node('d', 4));
	enqueue(&heap, create_node('e', 2));

	while (heap.size > 0) {
		HuffmanNode* node = dequeue(&heap);
		printf("%c : %d\n", node->c ? node->c : '#', node->freq);
		free(node);
	}
}
