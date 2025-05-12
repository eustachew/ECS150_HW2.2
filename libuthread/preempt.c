#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sa;
struct sigaction old_action;
struct itimerval timer;
struct itimerval old_timer;
sigset_t preempt_sig_set;

void preempt_handler(int signum){
	(void)signum;
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigprocmask(SIG_BLOCK, &preempt_sig_set, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	sigprocmask(SIG_UNBLOCK, &preempt_sig_set, NULL);
}

void preempt_start(bool preempt) //Do
{
	/* TODO Phase 4 */
	if(!preempt){
		return;
	}

	sigemptyset(&preempt_sig_set);
	sigaddset(&preempt_sig_set, SIGVTALRM);

	sa.sa_handler = preempt_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGVTALRM, &sa, &old_action);

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000000 / HZ;
	timer.it_value = timer.it_interval;

	setitimer(ITIMER_VIRTUAL, &timer, &old_timer);

}

void preempt_stop(void) //Do 
{
	/* TODO Phase 4 */
	//Restore the previous signal handler and timer that existed before we implemented our custom one
	sigaction(SIGVTALRM, &old_action, NULL);
	setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
}

