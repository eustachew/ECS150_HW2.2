#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "private.h"
#include "sem.h"

struct semaphore {
	/* TODO Phase 3 */
	size_t count;
	queue_t waitQueue;

};

sem_t sem_create(size_t count) //Do
{
	/* TODO Phase 3 */
	sem_t sem = (sem_t)malloc(sizeof(struct semaphore));
	if(!sem)
		return NULL;
	sem->count = count;
	sem->waitQueue = queue_create();
	

	return sem;

}

int sem_destroy(sem_t sem) //Do
{
	/* TODO Phase 3 */
	if (!sem || queue_length(sem->waitQueue) > 0)
		return -1; // Don't destroy if there are waiting threads

	
	queue_destroy(sem->waitQueue);
	free(sem);

	return 0;

}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (!sem)
		return -1;
	
	preempt_disable();

	
	if (sem->count > 0) {
		sem->count--;
		preempt_enable();
	} 
	else {
		queue_enqueue(sem->waitQueue, uthread_current);
		uthread_block(); //switches context

		sem->count--;
	}

	preempt_enable();
	return 0;
	
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */

	if (!sem)
		return -1;
	
	preempt_disable();

	//increments count if no one is wating
	if (queue_length(sem->waitQueue)==0) {

		sem->count++;
		preempt_enable();

		return 0;
	}

	//unblocks one waiting thread
	struct uthread_tcb* unblockedThread;
	if (queue_dequeue(sem->waitQueue, (void**) &unblockedThread) < 0) {
		preempt_enable();
		return -1;
	}

	uthread_unblock(unblockedThread);
	preempt_enable();

	return 0;


}

