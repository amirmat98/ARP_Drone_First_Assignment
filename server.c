#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

// Shared memory key
#define SHAREMEMORY_KEY 1234
#define SEMAPHORE_KEY "/my_semaphore"

// Special value to indicate no key pressed
#define NO_KEY_PRESSED 0

// Functions
void create_shared_memory(){

}
void handle_input(int *shared_key, sem_t *semaphore){

}

int main() {
    // Initialize shared memory
    int shared_key;
    int *shared_memory;

    // Try to create a new shared memory segment
    if ((shared_key = shmget(SHAREMEMORY_KEY, sizeof(int), IPC_CREAT | 0666)) < 0) {
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
        printf("Server: Received key: %d\n", pressed_key);


        // Clear the shared memory after processing the key
        *shared_memory = NO_KEY_PRESSED;

        usleep(500000);  // Adjust the delay as needed
    }

    // Detach the shared memory segment
    shmdt(shared_memory);

    // Close and unlink the semaphore
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_KEY);

    return 0;
}