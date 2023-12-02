#include "interface.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>


int main()
{
    // Attach to shared memory for key presses
    int *ptr_key;        // Shared memory for Key pressing
    char *ptr_pos;        // Shared memory for Drone Position      
    sem_t *sem_key;       // Semaphore for key presses
    sem_t *sem_pos;       // Semaphore for drone positions

    // Shared memory for KEY PRESSING
    int shm_key_fd = shm_open(SHAREMEMORY_KEY, O_RDWR, 0666);
    ptr_key = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_key_fd, 0);


    // Shared memory for DRONE POSITION
    int shm_pos_fd = shm_open(SHAREMEMORY_POSITION, O_RDWR, 0666);
    ptr_pos = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_pos_fd, 0);

    sem_key = sem_open(SEMAPHORE_KEY, 0);
    sem_pos = sem_open(SEMAPHORE_POSITION, 0);


    // Initial drone position (middle of the blackboard)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;

    // Write initial drone position in its corresponding shared memory
    sprintf((char *)ptr_pos, "%d,%d", drone_x, drone_y);


    initscr();
    timeout(0); // Set non-blocking getch
    curs_set(0); // Hide the cursor from the terminal
    create_blackboard();


    // Initialize color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);


    while (1) 
    {
        create_blackboard(); // Redraw blackboard in case the screen changed
        draw_drone(drone_x, drone_y);
        handle_input(ptr_key, sem_key);
        /* Update drone position */
        //sem_wait(semaphorePos);
        sscanf(ptr_pos, "%d,%d", &drone_x, &drone_y); // Obtain the values of X,Y from shared memory
        //sem_post(semaphorePos);
        usleep(20000);
        continue;
    }
    endwin();

    // close shared memories
    close(shm_key_fd);
    close(shm_pos_fd);

    // Close and unlink the semaphore
    sem_close(sem_key);
    sem_close(sem_pos);

    return 0;
}

void create_blackboard()
{
    // Clear the screen
    clear();

    // Get the dimensions of the terminal window
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    // Draw a rectangular border using the box function
    box(stdscr, 0, 0);

    // Print a title in the center of the blackboard
    mvprintw(0, (max_x - 11) / 2, "Drone Control");

    // Refresh the screen to apply changes
    refresh();
}

void draw_drone(int drone_x, int drone_y)
{

    // Draw the center of the cross
    mvaddch(drone_y, drone_x, '+' | COLOR_PAIR(1));

    refresh();
}

void handle_input(int *shared_key, sem_t *semaphore)
{
    int ch;

    // Disable echoing
    noecho();

    if ((ch = getch()) != ERR)
    {
        // Debugging: Commented out the print statement
        // printf("Pressed key: %d\n", ch);

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