
/*************************************
 *
 * filename:	sem-sysv.c
 * description:	implements the system v semaphore interfaces
 * author:		john lin
 * date:		2020-02-26
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include "sem-sysv.h"

/*
#define T SysvSem_T
typedef struct T *T;
struct T {
	char	*pathname;
	int	id;
	int	semid;
};
#undef T
*/

/*
 * name: MODULE_FUN_NAME(SysvSem, create)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, create)(const char *pathname, int id, int res_num)
{
	key_t	key;
	int	semid;
	int	flags = 0;
 	union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
         } tmp;


	if (!pathname || res_num < 1) {
		fprintf(stderr, "arguments error, path: %s, res num: %d\n",
				pathname, res_num);
		return -2;
	}

	if ((key = ftok(pathname, id)) == -1) {
		perror("ftok");
		return -1;
	}

	flags = (IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	//if ((semid = semget(key, res_num, flags)) == -1) {
	if ((semid = semget(key, 1, flags)) == -1) {
		perror("semget");
		return -1;
	}

	tmp.val = res_num;
	if (semctl(semid, 0, SETVAL, tmp) == -1) {
		perror("semctl");
		close(semid);
		return -1;
	}

	close(semid);
	fprintf(stdout, "<%s> create sys v semaphore\n", __func__);
	fprintf(stdout, "<%s> path: %s, id: %d, res num: %d\n", 
			__func__, pathname, id, res_num);
	fprintf(stdout, "<%s> key: %d, id: %d\n", __func__, key, semid);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(SysvSem, destroy)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, destroy)(const char *pathname, int id)
{
	key_t	key;
	int	semid;
	int	flags = 0;

	if (!pathname) {
		fprintf(stderr, "arguments error, path: %s\n",
				pathname);
		return -2;
	}

	if ((key = ftok(pathname, id)) == -1) {
		perror("ftok");
		return -1;
	}

	flags = O_RDWR | S_IRUSR | S_IWUSR;
	//if ((semid = semget(key, res_num, flags)) == -1) {
	if ((semid = semget(key, 1, flags)) == -1) {
		perror("semget");
		return -1;
	}

	if (semctl(semid, 0, IPC_RMID, 0) == -1) { // will remove immediately, 
				//and wake up other processes waiting on semop
		perror("semctl");
		return -1;
	}

	close(semid);
	return unlink(pathname);
}

/*
 * name: MODULE_FUN_NAME(SysvSem, open)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, open)(const char *pathname, int id)
{
	key_t	key;
	int	semid;
	int	flags = 0;

	if (!pathname) {
		fprintf(stderr, "arguments error, path: %s\n",
				pathname);
		return -2;
	}

	if ((key = ftok(pathname, id)) == -1) {
		perror("ftok");
		return -1;
	}

	flags = S_IRUSR | S_IWUSR;
	//if ((semid = semget(key, res_num, flags)) == -1) {
	if ((semid = semget(key, 1, flags)) == -1) {
		perror("semget");
		return -1;
	}

	return semid;
}

/*
 * name: MODULE_FUN_NAME(SysvSem, close)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
void MODULE_FUN_NAME(SysvSem, close)(int semid)
{
	if (semid > 0) {
		fprintf(stdout, "close semid: %d\n", semid);
		close(semid);
	}
}

/*
 * name: MODULE_FUN_NAME(SysvSem, get)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, get)(int semid)
{
	struct sembuf semops;

	if (semid < 0) return -2;

	semops.sem_num 	= 0;
	semops.sem_op	= -1;
	semops.sem_flg	= SEM_UNDO;
	if (semop(semid, &semops, 1) == -1) {
		perror("semop");
		return -1;
	}

	return 0;
}

/*
 * name: MODULE_FUN_NAME(SysvSem, release)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, release)(int semid)
{
	struct sembuf semops;

	if (semid < 0) return -2;

	semops.sem_num 	= 0;
	semops.sem_op	= 1;
	semops.sem_flg	= SEM_UNDO;
	if (semop(semid, &semops, 1) == -1) {
		perror("semop");
		return -1;
	}

	return 0;
}


