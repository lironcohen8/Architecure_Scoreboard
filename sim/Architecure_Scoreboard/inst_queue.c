#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "inst_queue.h"


bool is_full(inst_queue_t* queue) {
	return queue->is_full;
}

bool is_empty(inst_queue_t* queue) {
	return queue->is_empty;
}

uint32_t top(inst_queue_t* queue) {
	if (queue->is_empty) {
		printf("Should check using is_empty() before top\n");
		exit(1);
	}
	return queue->inst_buff[queue->head_index];
}

void enqueue(inst_queue_t* queue, uint32_t val) {
	if (queue->is_full) {
		printf("Should check using is_full() before inserting\n");
		exit(1);
	}

	uint8_t new_tail = (queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE;
	queue->inst_buff[new_tail] = val;
	queue->tail_index = new_tail;

	queue->is_full = ((queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE == queue->head_index);
	queue->is_empty = false;
}

uint32_t dequeue(inst_queue_t* queue) {
	if (queue->is_empty) {
		printf("Should check using is_empty() before dequeueing\n");
		exit(1);
	}
	uint8_t old_head = queue->head_index;

	uint8_t new_head = (queue->head_index + 1) % INSTRUCTION_QUEUE_SIZE;
	queue->head_index = new_head;

	queue->is_empty = ((queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE == queue->head_index);
	queue->is_full = false;

	return queue->inst_buff[old_head];
}

void init_instruction_queue(inst_queue_t* queue) {
	memset(queue->inst_buff, 0, sizeof(queue->inst_buff));
	queue->head_index = 0;
	queue->tail_index = INSTRUCTION_QUEUE_SIZE - 1;
	queue->is_empty = true;
	queue->is_full = false;
}