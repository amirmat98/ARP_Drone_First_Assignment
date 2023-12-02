#ifndef KEY_MANAGER_H    
#define KEY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>


/*
    Determine what action should be performed from user input
    @param pressed_key 
    @returns Message with with pressed key
*/
char* determine_action(int pressed_key,  char *shared_act);


#endif // KEY_MANAGER_H
