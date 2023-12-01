#include "server.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>



int main() {
    // Attach to shared memory for key presses
    int sharedـkey;
    int *shared_memory;
    // Semaphore for key presses
    sem_t *semaphore;
    // Semaphore for drone positions
    sem_t *semaphore_pos;

    if ((sharedـkey = shmget(SHAREMEMORY_KEY_1, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shared_memory = shmat(sharedـkey, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Attach to semaphore for key presses
    semaphore = sem_open(SEMAPHORE_KEY_1, O_CREAT, 0666, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Attach to shared memory for drone positions
    int shared_pos;
    int *shared_position;

    if ((shared_pos = shmget(SHAREMEMORY_KEY_2, 2 * sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shared_position = shmat(shared_pos, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Attach to semaphore for drone positions
    semaphore_pos = sem_open(SEMAPHORE_KEY_2, O_CREAT, 0666, 0);
    if (semaphore_pos == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Main loop
    while (1) {
        /* SERVER HANDLING OF KEY PRESSED */
        sem_wait(semaphore);
        int pressed_key = *shared_memory;
        printf("Server: Received key: %d\n", pressed_key);

        // TODO: Handle the received key or dispatch it to other processes
        // Here goes RESET
        // Here goes SUSPEND
        // Here goes QUIT

        // Clear the shared memory after processing the key
        *shared_memory = NO_KEY_PRESSED;

        /*SERVER HANDLING OF DRONE POSITION*/
        sem_wait(semaphore_pos);
        int drone_x = shared_position[0];
        int drone_y = shared_position[1];
        sem_post(semaphore_pos);

        // TODO: Logic for handling drone positions...

        usleep(50000);  // Delay for testing purposes
    }

    // Detach the shared memory segments
    shmdt(shared_memory);
    shmdt(shared_position);

    // Close and unlink the semaphores
    sem_close(semaphore);
    sem_close(semaphore_pos);
    sem_unlink(SEMAPHORE_KEY_1);
    sem_unlink(SEMAPHORE_KEY_2);
    return 0;
}