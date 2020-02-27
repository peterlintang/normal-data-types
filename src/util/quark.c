/*
 * this implemenst the quark interfaces, which 
 * looks much like corenova. in face, some code
 * are copied from corenova. ^ ^
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "quark.h"

int kill_all_quarks = 0;
//extern uint32_t quark_count = 0;

/* for inner usage */
void _request_pause(quark_t *this)
{
	pthread_mutex_lock(&this->lock);
	pthread_cond_broadcast(&this->pause);
	pthread_cond_wait(&this->unpause, &this->lock);
	pthread_mutex_unlock(&this->lock);
}

void *_quark_core(void *arg)
{
	quark_t *this = (quark_t *)arg;

	pthread_mutex_lock(&this->lock);
	this->stat |= QUARK_STAT_RUNNING;
	pthread_cond_broadcast(&this->ready);
	pthread_mutex_unlock(&this->lock);

	pthread_detach(pthread_self());

	while (!(this->req & QUARK_REQ_STOP) && 
		!kill_all_quarks) {
		if (!(this->func)(this->data))
			break;
		if (this->req & QUARK_REQ_PAUSE) _request_pause(this);
		if (this->req & QUARK_REQ_ONCE) break;
	}

	pthread_mutex_lock(&this->lock);
	this->stat &= !QUARK_STAT_RUNNING;
	pthread_cond_broadcast(&this->dead);
	pthread_mutex_unlock(&this->lock);
	
	return NULL;
}

/*************************************************
 * quark interfaces
 **************************************************/

/* 
 * the caller should ensure that @func & @data
 * are effective
 */ 
quark_t *_new_quark(quark_func_t func, void *data)
{
	quark_t *quark = calloc(1, sizeof(quark_t));

	if (quark) {
		quark->parent = pthread_self();
		pthread_cond_init(&quark->ready, NULL);
		pthread_cond_init(&quark->dead, NULL);
		pthread_cond_init(&quark->pause, NULL);
		pthread_cond_init(&quark->unpause, NULL);
		pthread_mutex_init(&quark->lock, NULL);

		pthread_attr_init(&quark->attr);
		if (pthread_attr_setstacksize(&quark->attr, 
			QUARK_DEFAULT_STACK_SIZE) != 0) {
			//DEBUGP(DERR, "_new_quark", 
			//	"unable to set the stack size to %d\n", 
			//	QUARK_DEFAULT_STACK_SIZE);
		}
		
		quark->func = func;
		quark->data = data;
		strncpy(quark->name, "some quark", QUARK_MAX_NAMELEN);
//		quark_count++;
	}
	return quark;
}

/*
 * send a signal to the pthread
 */
void _kill_quark(quark_t *quark, int signo)
{
	if (quark && quark->life)
		pthread_kill(quark->life, signo);
}

int _spin_quark(quark_t *quark)
{
	if (!quark) return 0; /* false */
	if (quark->stat & QUARK_STAT_RUNNING) return 1; /* true */

	pthread_mutex_lock(&quark->lock);

	if (pthread_create(&quark->life, 
			&quark->attr, 
			_quark_core, 
			(void *)quark) == 0) {
//		DEBUGP(DDEBUG, "_spin_quark", 
//			"'%s' off and away!\n", 
//			quark->name);
		pthread_cond_wait(&quark->ready, &quark->lock);
		pthread_mutex_unlock(&quark->lock);
		return 1; 
	} else {
		//DEBUGP(DERR, "_spin_quark", "pthread_create error!\n");
		pthread_mutex_unlock(&quark->lock);
	}
	return 0;
}

void _setname_quark(quark_t *quark, char *name)
{
	if (quark && name) {
		strncpy(quark->name, name, QUARK_MAX_NAMELEN);
	}
}

void _once_quark(quark_t *quark)
{
	if (quark) {
		quark->req |= QUARK_REQ_ONCE;
		_spin_quark(quark);
	}
}

void _stop_quark(quark_t *quark, uint32_t timeout_ms)
{
	pthread_mutex_lock(&quark->lock);
	if (quark->stat & QUARK_STAT_RUNNING) {
		struct timeval  now;
		struct timespec to;
		gettimeofday(&now, NULL);

		to.tv_sec = now.tv_sec + timeout_ms / 1000;
		to.tv_nsec = (now.tv_usec * 1000) + 
				(timeout_ms % 1000 * 1000000);
		if (to.tv_nsec >= 1000000000) {
			to.tv_sec += 1;
			to.tv_nsec %= 1000000000;
		}

		quark->req |= QUARK_REQ_STOP;
		if (pthread_cond_timedwait(&quark->dead, &quark->lock,
				(const struct timespec *)&to) != 0) {
			_kill_quark(quark, SIGTERM);	/* if the thread is killed by SIGYERM, 
							what will happen if we still timedwait ? */
			to.tv_nsec += 1000000;
			if (to.tv_nsec >= 1000000000) {
				to.tv_sec += 1;
				to.tv_nsec %= 1000000000;
			}
			if (pthread_cond_timedwait(&quark->dead, &quark->lock,  /**  why why why ???? ***/
				(const struct timespec *)&to) != 0) 
				pthread_cancel(quark->life);

			quark->stat &= !QUARK_STAT_RUNNING;
		}
	}
	pthread_mutex_unlock(&quark->lock);
}

void _pause_quark(quark_t *quark)
{
	if (!quark) return ;

	pthread_mutex_lock(&quark->lock);
	if (quark->stat & QUARK_STAT_RUNNING) {
		quark->req |= QUARK_REQ_PAUSE;
		pthread_cond_wait(&quark->pause, &quark->lock);
		quark->req = 0;
		quark->stat |= QUARK_STAT_PAUSED;
	}
	pthread_mutex_unlock(&quark->lock);
}

void _unpause_quark(quark_t *quark)
{
	if (!quark) return ;

	pthread_mutex_lock(&quark->lock);
	if (quark->stat & QUARK_STAT_PAUSED) {
		quark->req = 0;
		pthread_cond_broadcast(&quark->unpause);
	}
	pthread_mutex_unlock(&quark->lock);
}

void _set_kill_all_quarks(void) 
{
	kill_all_quarks = 1;
}

void _destroy_quark(quark_t **quark_ptr)
{
	if (!quark_ptr || !(*quark_ptr)) return ;

	pthread_mutex_destroy(&(*quark_ptr)->lock);
	pthread_cond_destroy(&(*quark_ptr)->ready);
	pthread_cond_destroy(&(*quark_ptr)->dead);
	pthread_cond_destroy(&(*quark_ptr)->pause);
	pthread_cond_destroy(&(*quark_ptr)->unpause);
	pthread_attr_destroy(&(*quark_ptr)->attr);

//	quark_count--;
	free(*quark_ptr);
	*quark_ptr = NULL;
}


