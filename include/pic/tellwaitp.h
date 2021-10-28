
/*
 * 使用管道实现父子进程之间的同步原语
 */
#ifndef CI_TELL_WAIT_P_H
#define CI_TELL_WAIT_P_H


void TELL_WAIT_P(void);

void TELL_PARENT_P(pid_t pid);

void WAIT_PARENT_P(void);

void TELL_CHILD_P(pid_t pid);

void WAIT_CHILD_P(void);

#endif // CI_TELL_WAIT_P_H


