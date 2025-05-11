#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct task {
	/* TODO Phase 1 */
	void *data;
	task_t *next;

};
struct queue {
	/* TODO Phase 1 */
	int size;
	task_t *head;
	task_t *tail;

};

void free_custom(queue_t queue, void *data){
	(void)queue;
	free(data);
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t new_queue = malloc(sizeof(struct queue));
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_queue->size = 0;
	return new_queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL || queue->size > 0){
		return -1;
	}
	queue_iterate(queue, free_custom);
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL){
		return -1;
	}
	else{

		task_t *new_task = malloc(sizeof(task_t));
		new_task->data = data;
		new_task->next = NULL;

		if(queue->size == 0){
			queue->head = new_task;
			queue->tail = new_task;
			queue->size++; 
		}
		else{
			queue->tail->next = new_task;
			queue->tail = new_task;
			queue->size++;
		}
		return 0;
	}
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(data == NULL || queue == NULL || queue->size == 0){
		return -1;
	}
	else{
		if(queue->size == 1){
			*data = queue->head->data;
			free(queue->head);
			queue->size--;
			//queue->head == NULL;
			//queue->tail == NULL;
		}
		else{
			task_t *temp = queue->head->next;
			*data = queue->head->data;
			free(queue->head);
			queue->head = temp;
			queue->size--;
		}
	}
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(data == NULL || queue == NULL || queue->size == 0){
		return -1;
	}

	if(queue->size == 1){
		if(queue->head->data != data){
			return -1;
		}
		else{
			queue->head = NULL;
			queue->tail = NULL;
			free(queue->head);
			queue->size--;
		}
	}

	task_t *current_node = queue->head;
	task_t *previous_node = NULL;

	while(current_node != NULL){
		if(current_node->data == data){
			break;
		}
		previous_node = current_node;
		current_node = current_node->next;
	}

	if(current_node == NULL){
		return -1;
	}

	if(current_node == queue->head){
		queue->head = current_node->next;
	}

	if(current_node == queue->tail){
		queue->tail = previous_node;
	}
	else{
		previous_node->next = current_node->next;
	}

	free(current_node);
	current_node = NULL;
	queue->size--;
	return 0;
	
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if(func == NULL || queue == NULL ){
		return -1;
	}

	task_t *current_node = queue->head;
	task_t *next_node = NULL;

	if(queue->size == 1){
		func(queue, queue->head->data);
	}
	else{
		while(current_node != NULL){
			next_node = current_node->next;
			func(queue, current_node->data);
			current_node = next_node;
		}
	}
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL){
		return -1;
	}
	else{
		return queue->size;
	}
}

