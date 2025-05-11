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
	int *stack_pointer;

};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return current_thread;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

void uthread_exit(void) //Do
{
	/* TODO Phase 2 */
	current_thread->state = COMPLETED;
	uthread_yield();

}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	preempt_disable();

	//creating the new thread itself
	struct uthread_tcb* thread = malloc(sizeof(struct uthread_tcb));
	thread->context = (uthread_ctx_t*)(malloc(sizeof(uthread_ctx_t)));
	thread->stack_pointer = uthread_ctx_alloc_stack;

	if(uthread_ctx_init(thread->context, thread->stack_pointer, func, arg) == -1){
		return -1;
	}

	thread->state = READY;
	thread->id = assign_thread_ID;

	//queueing the new thread
	if(queue_enqueue(ready_queue, thread) == -1 ){
		return -1;
	}

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
	idle_thread->id = assign_thread_ID;
	idle_thread->stack_pointer = NULL;
	idle_thread->state = RUNNING;
	current_thread = idle_thread;

	//Create out initial thread
	uthread_create(func, arg);

	//Start preemption if it is requested
	preempt_start(preempt);

	while(queue_length(ready_queue) > 0){
		uthread_yield();
	}

	//Stop preemption since now we have no more threads to run
	preempt_stop();

	while(queue_length(completed_queue) > 0){
		
		(queue_dequeue());
	}




}

void uthread_block(void) //Do
{
	/* TODO Phase 3 */
	current_thread->state = BLOCKED;
	queue_enqueue(blocked_queue, current_thread);
	uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread) //Do
{
	/* TODO Phase 3 */
	if (uthread->state == blocked_queue) {
		uthread->state = READY;
		queue_enqueue(ready_queue, uthread);
	}
}

