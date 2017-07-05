#ifndef __SEM_COM__H_
#define __SEM_COM__H_

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int init_sem(int sem_id, int init_value);
int del_sem(int sem_id);
int sem_p(int sem_id);
int sem_v(int sem_id);

#endif