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
	task_t **tasks;
	task_t *head;
	task_t *tail;

};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t new_queue = malloc(sizeof(struct queue));
	new_queue->tasks = NULL;
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_queue->size = 0;
	return new_queue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL){
		return -1;
	}
	for(int i = 0; i < queue->size; i++){
		free(queue->tasks[i]);
	}
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
		if(queue->size == 0){
			queue->size += 1;
			queue->head = new_task;
			queue->tail = new_task;
			queue->tasks = new_task;
		}
		else if(queue->size == 1){
			queue->size += 1;
			queue->tail->next = new_task;
			queue->head = new_task;
			queue->tasks = new_task;
		}
		else{
			queue->size += 1;
			queue->head->next = new_task;
			queue->head = new_task;
			queue->tasks = new_task;
		}
		return 0;
	}
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

