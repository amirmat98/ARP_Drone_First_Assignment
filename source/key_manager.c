#include "key_manager.h"
#include "constants.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <ctype.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>


int main()
{
    // Shared memory variables
    void *ptr_key;       
    void *ptr_action;            
    int shared_key;
    int shared_action;

    // Semaphores
    sem_t *sem_key;  
    sem_t *sem_action;       

    // Initialize shared memory for KEY PRESSING
    shared_key = shm_open(SHAREMEMORY_KEY, O_RDWR, 0666);
    ptr_key = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shared_key, 0);

    // Initialize shared memory for DRONE CONTROL - ACTION
    shared_action = shm_open(SHAREMEMORY_ACTION, O_RDWR, 0666);
    ptr_action = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shared_action, 0);

    // Initialize semaphores
    sem_key = sem_open(SEMAPHORE_KEY, 0);
    sem_action = sem_open(SEMAPHORE_ACTION, 0);

    // Main loop
    /*THIS SECTION IS FOR OBTAINING KEY INPUT*/
    while (1) 
    {
        /*THIS SECTION IS FOR OBTAINING KEY INPUT*/

        sem_wait(sem_key);  // Wait for the semaphore to be signaled from interface.c process
        int pressed_key = *(int*)ptr_key;    // Read the pressed key from shared memory 
        printf("Pressed key: %c\n", (char)pressed_key);
        fflush(stdout);

        /*THIS SECTION IS FOR DRONE ACTION DECISION*/

        char *action = determine_action(pressed_key, ptr_action);
        printf("Action sent to drone: %s\n\n", action);
        fflush(stdout);

        shared_key = NO_KEY_PRESSED; // Clear the shared memory after processing the key
    }


    // close shared memories
    close(shared_key);
    close(shared_action);

    // Close and unlink the semaphore
    sem_close(sem_key);
    sem_close(sem_action);


    return 0;
}


// US Keyboard assumed
char* determine_action(int pressed_key, char *shared_action)
{
    char key = toupper(pressed_key);
    int x; int y;

    // Disclaimer: Y axis is inverted on tested terminal.
    if ( key == 'W' || key == 'I')
    {
        x = 0;    // Movement on the X axis.
        y = -1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "UP";
    }
    if ( key == 'X' || key == ',')
    {
        x = 0;    // Movement on the X axis.
        y = 1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "DOWN";
    }
    if ( key == 'A' || key == 'J')
    {
        x = -1;    // Movement on the X axis.
        y = 0;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "LEFT";
    }
    if ( key == 'D' || key == 'L')
    {
        x = 1;    // Movement on the X axis.
        y = 0;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "RIGHT";
    }
    if ( key == 'Q' || key == 'U')
    {
        x = -1;    // Movement on the X axis.
        y = -1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "UP-LEFT";
    }
    if ( key == 'E' || key == 'O')
    {
        x = 1;    // Movement on the X axis.
        y = -1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "UP-RIGHT";
    }
    if ( key == 'Z' || key == 'M')
    {
        x = -1;    // Movement on the X axis.
        y = 1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "DOWN-LEFT";
    }
    if ( key == 'C' || key == '.')
    {
        x = 1;    // Movement on the X axis.
        y = 1;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "DOWN-RIGHT";
    }
    if ( key == 'S' || key == 'K')
    {
        x = 900;    // Special value interpreted by drone.c process
        y = 0;
        sprintf(shared_action, "%d,%d", x, y);
        return "STOP";
    }
    else
    {
        return "None";
    }

}
