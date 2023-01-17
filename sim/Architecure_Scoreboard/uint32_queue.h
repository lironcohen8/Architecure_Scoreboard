#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INSTRUCTION_QUEUE_SIZE			16

/* defines the instructions queue. */
typedef struct {
	uint32_t buff[INSTRUCTION_QUEUE_SIZE];
	uint8_t head_index;
	uint8_t tail_index;

	bool is_full;
	bool is_empty;
} uint32_queue_t;

/* checks if the queue is full. */
bool is_full(uint32_queue_t* queue);

/* checks if the queue is empty. */
bool is_empty(uint32_queue_t* queue);

/* if the queue is not empty, returns the instruction in the head of the buffer. */
uint32_t top(uint32_queue_t* queue);

/* if the queue is not full, add an instruction into its tail. */
void enqueue(uint32_queue_t* queue, uint32_t val);

/* if the queue is not empty, removes the value in its head and returns it. */
uint32_t dequeue(uint32_queue_t* queue);

/* setting memory for the queue and initializing its values. */
void init_queue(uint32_queue_t* queue);