#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>


// Function prototypes

/*
Create shared memory
    @param name path to shared memory
    @returns ptr to shared memory
*/

void *create_shm(char *name);
void handle_input(int *shared_key, sem_t *semaphore);

#endif // SERVER_H
