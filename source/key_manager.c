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

    // Main loop
    while (1) {
        // Wait for the semaphore to be signaled
        sem_wait(semaphore);

        // Read the pressed key from shared memory
        int pressed_key = *shared_memory;
        printf("Pressed key: %c\n", (char)pressed_key);

        // Determine the action based on the pressed key
        char *action = determine_action(pressed_key);

        // Send the action to the drone program
        send_action_to_drone(action);

        // Clear the shared memory after processing the key
        clear_shared_memory(shared_memory);
        //usleep(500000);
    }


    // Detach the shared memory segment
    shmdt(shared_memory);
    // Close and unlink the semaphore
    sem_close(semaphore);

    return 0;
}


char* determine_action(int pressed_key){

    char key = toupper(pressed_key);

        if ( key == 'W' || key == 'I')
    {
        return "Up";
    }
    if ( key == 'X' || key == ',')
    {
        return "Down";
    }
    if ( key == 'A' || key == 'J')
    {
        return "Right";
    }
    if ( key == 'D' || key == 'L')
    {
        return "Left";
    }
    if ( key == 'Q' || key == 'U')
    {
        return "UP-LEFT";
    }
    if ( key == 'E' || key == 'O')
    {
        return "UP-RIGHT";
    }
    if ( key == 'Z' || key == 'M')
    {
        return "DOWN-LEFT";
    }
    if ( key == 'C' || key == '.')
    {
        return "DOWN-RIGHT";
    }
    if ( key == 'S' || key == 'K')
    {
        return "STOP";
    }
    else
    {
        return "None";
    }
}

void send_action_to_drone(char* action){
    // Here you can implement the code to send the action to the drone program
    // For now, let's print the action to the standard output
    printf("Action sent to drone: %s\n\n", action);
    fflush(stdout);
}

void clear_shared_memory(int* shared_memory){
    // Set the shared memory to a special value to indicate no key pressed
    *shared_memory = NO_KEY_PRESSED;

}


