#ifndef __SHM_COM__H_
#define __SHM_COM__H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_BUFF_SZ 2048

struct shm_buff
{
    int pid;
    char buffer[SHM_BUFF_SZ];
};

#endif