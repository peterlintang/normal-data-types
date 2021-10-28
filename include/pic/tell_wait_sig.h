

/*
 *  process sync implemented by signal
 *  父子进程 同 步, 用 信号 实现 
 *  used by parent & child process mainly
 */
#ifndef TELL_WAIT_SIGNAL_H
#define TELL_WAIT_SIGNAL_H

void TELL_WAIT_S(void);

void TELL_PARENT_S(pid_t pid);

void TELL_CHILD_S(pid_t pid);

void WAIT_PARENT_S(void);

void WAIT_CHILD_S(void);


#endif //  	end of TELL_WAIT_SIGNAL_H




