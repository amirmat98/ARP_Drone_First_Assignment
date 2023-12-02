#include "key_manager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>

int main(){
    // Initialize shared memory
    int shared_key;
    int *shared_memory;

    // Try to access the existing shared memory segment
    if ((shared_key = shmget(SHAREMEMORY_KEY_1, sizeof(int), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    if ((shared_memory = shmat(shared_key, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore
    sem_t *semaphore = sem_open(SEMAPHORE_KEY_1, O_CREAT, 0666, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Initialize shared memory 3: 'Action'
    int shared_act;
    char *shared_action;

    if ((shared_act = shmget(SHAREMEMORY_KEY_3, 80*sizeof(char), IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    if ((shared_action = shmat(shared_act, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    
    sem_t *semaphore_act = sem_open(SEMAPHORE_KEY_3, O_CREAT, 0666, 0);
    if (semaphore_act == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }



    // Main loop
    /*THIS SECTION IS FOR OBTAINING KEY INPUT*/
    while (1) {
        // Wait for the semaphore to be signaled
        sem_wait(semaphore);

        // Read the pressed key from shared memory
        int pressed_key = *shared_memory;
        printf("Pressed key: %c\n", (char)pressed_key);
        // Clear the shared memory after processing the key
        clearSharedMemory(sharedMemory);

        // Determine the action based on the pressed key
        char *action = determine_action(pressed_key, shared_action);

        /*THUS SECTION IS FOR DRONE ACTION DECISION*/

        // Determine the action based on the pressed key
        char *action = determine_action(pressed_key, shared_action);
        // Print the action taken
        printf("Action sent to drone: %s\n\n", action);
        fflush(stdout);
    }


    // Detach the shared memory segment
    shmdt(shared_memory);
    shmdt(shared_action);
    // Close and unlink the semaphore
    sem_close(semaphore);
    sem_close(semaphore_act);

    return 0;
}


char* determine_action(int pressed_key, char *shared_action){

    char key = toupper(pressed_key);
    int x; int y;

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
        return "RIGHT";
    }
    if ( key == 'D' || key == 'L')
    {
        x = 1;    // Movement on the X axis.
        y = 0;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "LEFT";
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
        x = 0;    // Movement on the X axis.
        y = 0;    // Movement on the Y axis.
        sprintf(shared_action, "%d,%d", x, y);
        return "STOP";
    }
    else
    {
        return "None";
    }

}


/*
void send_action_to_drone(char* action){
    // Here you can implement the code to send the action to the drone program
    // For now, let's print the action to the standard output
    printf("Action sent to drone: %s\n\n", action);
    fflush(stdout);
}
*/

void clear_shared_memory(int* shared_memory)
{
    // Set the shared memory to a special value to indicate no key pressed
    *shared_memory = NO_KEY_PRESSED;
}


