#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>


/*
    Creates shared memory segments 
    This is divieded into two parts.
    he first part creates the shared memory segment with predefined functions and size. 
    The second part maps the memory and returns the ptr. 
    @param name Name of the shared memory segment    
    @returns pointer to the memory map of type void
*/
void signal_handler(int signo, siginfo_t *siginfo, void *context);


/*
    Closes and unlinks semaphores, unmaps and unlinks shared memory segments
*/
void *create_shm(char *name);

void clean_up();

#endif // SERVER_H
