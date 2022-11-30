#pragma once

#include <stdint.h>
#include <stdbool.h>

#define INSTRUCTION_QUEUE_SIZE			16

typedef struct {
	uint32_t inst_buff[INSTRUCTION_QUEUE_SIZE];
	uint8_t head_index;
	uint8_t tail_index;

	bool is_full;
	bool is_empty;
} inst_queue_t;


bool is_full(inst_queue_t* queue);

bool is_empty(inst_queue_t* queue);

uint32_t top(inst_queue_t* queue);

void enqueue(inst_queue_t* queue, uint32_t val);

uint32_t dequeue(inst_queue_t* queue);

void init_instruction_queue(inst_queue_t* queue);