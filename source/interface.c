#include "interface.h"
#include "constants.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>

int main(){
    initscr();
    timeout(0); // Set non-blocking getch
    curs_set(0); // Hide the cursor from the terminal
    create_blackboard();

    // Initialize shared memory
    int shared_key;
    int *shared_memory;
    // Semaphore for drone positions    
    sem_t *semaphore, *semaphore_pos;

    // Try to create a new shared memory segment
    if ((shared_key = shmget(SHAREMEMORY_KEY_1, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    if ((shared_memory = shmat(shared_key, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore for key presses
    sem_t *semaphore = sem_open(SEMAPHORE_KEY_1, O_CREAT, 0666, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Initialize shared memory for drone positions
    int shared_pos;
    int *shared_position;

    if ((shared_pos = shmget(SEMAPHORE_KEY_2, 2 * sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shared_position = shmat(shared_pos, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphore for drone positions
    sem_t *semaphore_pos = sem_open(SEMAPHORE_KEY_2, O_CREAT, 0666, 0);
    if (semaphore_pos == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    

    // Initial drone position (middle of the blackboard)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;


    // Write initial drone position in its corresponding shared memory
    /*
    int drone_position[2];
    drone_position[0] = drone_x;
    drone_position[1] = drone_y;
    initialize_drone_position(shared_position, drone_position);
    */
    sprintf(shared_position, "%d,%d", drone_x, drone_y);

    // Initialize color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);


    while (1) {
        printf("Entered while loop");
        create_blackboard();
        draw_drone(drone_x, drone_y);
        handle_input(shared_memory, semaphore);
        //Update drone position
        //sem_wait(semaphorePos);
        //drone_position[0] = shared_position[0];
        //drone_position[1] = shared_osition[1];
        sscanf(shared_position, "%d,%d", &drone_x, &drone_y); // Obtain the values of x,y from shared memory
        //sem_post(semaphorePos);

        usleep(20000);
        continue;
    }

    // Detach the shared memory segment
    shmdt(shared_memory);

    // Close and unlink the semaphore
    sem_close(semaphore);

    // End the shared memory segment
    shmctl(shared_key, IPC_RMID, NULL);

    endwin();
    return 0;

}


void create_blackboard(){
    // Clear the screen
    clear();

    // Get the dimensions of the terminal window
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw a rectangular border using the box function
    box(stdscr, 0, 0);

    // Print a title in the center of the blackboard
    mvprintw(0, (max_x - 11) / 2, "Drone Control");

    // Refresh the screen to apply changes
    refresh();
}

void draw_drone(int drone_x, int drone_y){
    // Draw the center of the cross
    mvaddch(drone_y, drone_x, '+' | COLOR_PAIR(1));

    refresh();
}

void handle_input(int *shared_key, sem_t *semaphore){
    int ch;

    // Disable echoing
    noecho();

    if ( (ch = getch() ) != ERR) {
        // Debugging: Print the pressed key
        //printf("Pressed key: %d\n", ch);

        // Store the pressed key in shared memory
        *shared_key = ch;

        // Signal the semaphore to notify the server
        sem_post(semaphore);
    }

    // Enable echoing
    echo();

    // Clear the input buffer
    flushinp();
}

/*
void initialize_drone_position(int *shared_pos, int drone_position[2]) {

    //sem_wait(semaphorePos);
    shared_pos[0] = drone_position[0];
    shared_pos[1] = drone_position[1];
    //sem_post(semaphorePos);
}
*/