#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>

// Shared memory key
#define SHAREMEMORY_KEY 1234
#define SEMAPHORE_KEY "/my_semaphore"

// Special value to indicate no key pressed
#define NO_KEY_PRESSED 0

char* determine_action(int pressed_key){

    // Determine the action based on the pressed key
    switch ( toupper ( (char) pressed_key)){
        case 'W':
            return "Up";
        case 'S':
            return "Down";
        case 'A':
            return "Left";
        case 'D':
            return "Right";
        case 'Q':
            return "Up-Left";
        case 'E':
            return "Up-Right";
        case 'Z':
            return "Down-Left";
        case 'C':
            return "Down-Right";
        case 'X':
            return "STOP";
        case 'R':
            return "Reset";
        case 'T':
            return "Suspend";
        case 'Y':
            return "Quit";
        default:
            return "None";
    }
}

void send_action_to_drone(char* action){
    // Here you can implement the code to send the action to the drone program
    // For now, let's print the action to the standard output
    printf("Action sent to drone: %s\n\n", action);
}

void clear_shared_memory(int* shared_memory){
    // Set the shared memory to a special value to indicate no key pressed
    *shared_memory = NO_KEY_PRESSED;

}

int main(){
    // Initialize shared memory
    int shared_key;
    int *shared_memory;

    // Try to access the existing shared memory segment
    if ((shared_key = shmget(SHAREMEMORY_KEY, sizeof(int), 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    if ((shared_memory = shmat(shared_key, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore
    sem_t *semaphore = sem_open(SEMAPHORE_KEY, O_CREAT, 0666, 0);
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
        usleep(500000);
    }


    // Detach the shared memory segment
    shmdt(shared_memory);
    // Close and unlink the semaphore
    sem_close(semaphore);

    return 0;
}
