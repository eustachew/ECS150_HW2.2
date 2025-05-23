#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define COMPLETED 3

queue_t ready_queue;
queue_t blocked_queue;
queue_t completed_queue; //Zombie//

struct uthread_tcb* current_thread; 

int assign_thread_ID(){
	static int available_ID = -1;
	available_ID++;
	return available_ID;
}

struct uthread_tcb {
	/* TODO Phase 2 */
	int id;
	uthread_ctx_t *context;
	int state;
	void *stack_pointer;

};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return current_thread;
}


void printStates(){
	printf("Blocked queue length: %d\n", queue_length(blocked_queue));
	printf("Ready queue length: %d\n", queue_length(ready_queue));
	printf("Completed queue length: %d\n", queue_length(completed_queue));
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	preempt_disable();

	//printStates();

	struct uthread_tcb *nextThread;
	struct uthread_tcb *prevThread = current_thread;
	//printf("Current thread ID: %d, current thread state: %d\n",current_thread->id, current_thread->state);
	//printf("Thread ID = %d is yielding\n", current_thread->id);

	//save current thread to ready queue if not completed
	if (prevThread->state == RUNNING) {
		//printf("Thread to yield is RUNNING\n");
		prevThread->state = READY;
		queue_enqueue(ready_queue, prevThread);
	}
	//printf("Thread to yield is not RUNNING\n");
	//dequeue next thread from ready queue & switch context to that thread
	if(queue_dequeue(ready_queue, (void**)&nextThread) == -1){
		preempt_enable();
		return;
	}
	//printf("Dequeued and running thread ID = %d\n", nextThread->id);
	nextThread->state = RUNNING;
	current_thread = nextThread;

	//printf("Thread to run now ID: %d\n", current_thread->id);

	//printf("Switching from ID:%d to ID:%d\n", prevThread->id, nextThread->id);
	uthread_ctx_switch(prevThread->context, nextThread->context);

	preempt_enable();

}

void uthread_exit(void) //Do
{
	/* TODO Phase 2 */
	preempt_disable();

	current_thread->state = COMPLETED;
	
	queue_enqueue(completed_queue, current_thread);

	preempt_enable();

	uthread_yield();

}

int uthread_create(uthread_func_t func, void *arg)
{
		/* TODO Phase 2 */
	preempt_disable();

	//creating the new thread itself
	struct uthread_tcb* thread = malloc(sizeof(struct uthread_tcb));

	thread->context = (uthread_ctx_t*)(malloc(sizeof(uthread_ctx_t)));

	thread->stack_pointer = uthread_ctx_alloc_stack();

	if(uthread_ctx_init(thread->context, thread->stack_pointer, func, arg) == -1){
		return -1;
	}

	thread->state = READY;
	thread->id = assign_thread_ID();
	//printf("Created thread ID = %d\n", thread->id);

	//queueing the new thread
	if(queue_enqueue(ready_queue, thread) == -1 ){
		//printf("Enqueue of new thread failed\n");
		return -1;
	}
	//printf("Thread ID = %d as been added to ready queue\n", thread->id);

	preempt_enable();

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	//Creating the necessary queues
	ready_queue = queue_create();
	blocked_queue = queue_create();
	completed_queue = queue_create();

	if(ready_queue == NULL || blocked_queue == NULL || completed_queue == NULL){
		return -1;
	}

	//Registering the main thread as the "idle" thread
	struct uthread_tcb *idle_thread = malloc(sizeof(struct uthread_tcb));


	idle_thread->context = (uthread_ctx_t*)(malloc(sizeof(uthread_ctx_t)));

	idle_thread->id = assign_thread_ID();
	idle_thread->stack_pointer = NULL;
	idle_thread->state = RUNNING;
	current_thread = idle_thread;

	//Create out initial thread
	if(uthread_create(func, arg) == -1){
		return -1;
	}

	//Start preemption if it is requested
	preempt_start(preempt);

	while(queue_length(ready_queue) > 0){
		//printf("Current Thread (ID:%d) is yielding\n", current_thread->id);
		//printf("Ready queue length: %d\n", queue_length(ready_queue));
		uthread_yield();
		//printf("About to start next iteration\n");
	}
	//Stop preemption since now we have no more threads to run
	preempt_stop();

	//free all memory
	queue_iterate(completed_queue, free_custom);
	queue_destroy(completed_queue);
	queue_destroy(blocked_queue);
	queue_destroy(ready_queue);
	free(current_thread->context);
	free(current_thread);

	return 0;
}

void uthread_block(void) //Do
{
	/* TODO Phase 3 */
	//printf("Blocking Thread ID = %d\n", current_thread->id);
	preempt_disable();

	current_thread->state = BLOCKED;
	
	queue_enqueue(blocked_queue, current_thread);

	preempt_enable();

	uthread_yield();

}

void uthread_unblock(struct uthread_tcb *uthread) //Do
{
	/* TODO Phase 3 */
	preempt_disable();

	if (uthread->state == BLOCKED) {
		//printf("Unblocking ID = %d\n", uthread->id);
		uthread->state = READY;
		queue_delete(blocked_queue, uthread);
		queue_enqueue(ready_queue, uthread);
	}
	preempt_enable();
}

