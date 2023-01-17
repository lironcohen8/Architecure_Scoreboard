#include "uint32_queue.h"

/* checks if the queue is full. */
bool is_full(uint32_queue_t* queue) {
	return queue->is_full;
}

/* checks if the queue is empty. */
bool is_empty(uint32_queue_t* queue) {
	return queue->is_empty;
}

/* if the queue is not empty, returns the instruction  in the head of the buffer. */
uint32_t top(uint32_queue_t* queue) {
	if (queue->is_empty) {
		printf("Should check using is_empty() before top\n");
		exit(1);
	}
	return queue->buff[queue->head_index];
}

/* if the queue is not full, add an instruction into its tail. */
void enqueue(uint32_queue_t* queue, uint32_t val) {
	if (queue->is_full) {
		printf("Should check using is_full() before inserting\n");
		exit(1);
	}

	uint8_t new_tail = (queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE;
	queue->buff[new_tail] = val;
	queue->tail_index = new_tail;

	queue->is_full = ((queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE == queue->head_index);
	queue->is_empty = false;
}

/* if the queue is not empty, removes the value in its head and returns it. */
uint32_t dequeue(uint32_queue_t* queue) {
	if (queue->is_empty) {
		printf("Should check using is_empty() before dequeueing\n");
		exit(1);
	}
	uint8_t old_head = queue->head_index;

	uint8_t new_head = (queue->head_index + 1) % INSTRUCTION_QUEUE_SIZE;
	queue->head_index = new_head;

	queue->is_empty = ((queue->tail_index + 1) % INSTRUCTION_QUEUE_SIZE == queue->head_index);
	queue->is_full = false;

	return queue->buff[old_head];
}

/* setting memory for the queue and initializing its values. */
void init_queue(uint32_queue_t* queue) {
	memset(queue->buff, 0, sizeof(queue->buff));
	queue->head_index = 0;
	queue->tail_index = INSTRUCTION_QUEUE_SIZE - 1;
	queue->is_empty = true;
	queue->is_full = false;
}