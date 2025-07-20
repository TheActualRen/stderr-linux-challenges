#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO:
// 1) Populate a Huffman Tree so that I can get the coded bits for a character
//		- Need to build a BINARY_TREE
//		- So I need to implement a PRIORITY_QUEUE
//		- For the PRIORITY_QUEUE will need a linked list struct and queue struct
//		- For the BINARY_TREE I will need a tree node struct

typedef struct QueueNode {
	int val;
	struct QueueNode* next;
} QueueNode;

typedef struct {
	QueueNode* head;
	QueueNode* tail;
	int size;
} Queue;

Queue* create_queue() {
	Queue* q = malloc(sizeof(Queue));

	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

	return q;
}

int size(Queue* q) { 
	return q->size; 
}

bool is_empty(Queue* q) {
	return q->size == 0; 
}

int peek(Queue* q, bool* status) {
	if (is_empty(q)) {
		*status = false;
	}

	*status = true;
	return q->head->val;
}

void enqueue(Queue* q, int newVal) {
	QueueNode* newNode = malloc(sizeof(QueueNode));
	newNode->val = newVal;
	newNode->next = NULL;

	if (is_empty(q)) {
		q->head = newNode;
		q->tail = newNode;
	} else {
		q->tail->next = newNode;
		q->tail = q->tail->next;
	}

	q->size++;
}

int dequeue(Queue* q, bool* status) {
	if (is_empty(q)) {
		*status = false;
		return 0;
	}

	*status = true;
	int value = q->head->val;
	QueueNode* temp = q->head;

	if (q->size == 1) {
		q->head = NULL;
		q->tail = NULL;
	} else {
		q->head = q->head->next;
	}

	free(temp);
	q->size--;

	return value;
}

void destroy_queue(Queue* q) {
	QueueNode* curr = q->head;

	while (curr != NULL) {
		QueueNode* temp = curr;
		curr = curr->next;
		free(temp);
	}

	free(q);
}

void printQueue(Queue* q) {
	QueueNode* curr = q->head;

	while (curr != NULL) {
		printf("%d ", curr->val);
		curr = curr->next;
	}
	printf("\n");
}

int main() {
	Queue* myqueue = create_queue();

	if (is_empty(myqueue)) {
		printf("The Queue is empty.\n");
	}

	for (int i = 1; i < 6; i++) {
		enqueue(myqueue, i);
	}
	printQueue(myqueue);

	bool status = true;
	int myval = dequeue(myqueue, &status);

	if (status == true) {
		printf("My Val: %d\n", myval);
	}

	printQueue(myqueue);
	int front = peek(myqueue, &status);

	if (status == true) {
		printf("front: %d\n", front);
	}

	destroy_queue(myqueue);
}
