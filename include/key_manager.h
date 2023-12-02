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


/**
 * Determine what action should be performed from user input
 * 
 * @param pressedKey 
 * @returns Message with with pressed key
*/
char* determine_action(int pressed_key, char *shared_act);

/**
 * Sends action to Drone
 * 
 * @param action translated action
*/
//void send_action_to_drone(char* action);

/**
 * Clears shared Memory 
 * 
 * @param sharedMemory memory to be cleaned
*/
void clear_shared_memory(int* shared_memory);


#endif // KEY_MANAGER_H
