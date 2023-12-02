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

    /* INITIALIZATION AND EXECUTION OF NCURSES FUNCTIONS */
    initscr(); // Initialize
    timeout(0); // Set non-blocking getch
    curs_set(0); // Hide the cursor from the terminal
    //create_window();  // Create the windows on the spawned 'Konsole' terminal.
    start_color(); // Initialize color for drawing drone
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // Drone will be of color blue

    // Set the initial drone position (middle of the window)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;

    // Write initial drone position in its corresponding shared memory
    sprintf(ptr_pos, "%d,%d,%d,%d", drone_x, drone_y, max_x, max_y);




    while (1) 
    {
        sem_wait(sem_pos); // Wait for the semaphore to be signaled from drone.c process
        // Obtain the position values from shared memory
        sscanf(ptr_pos, "%d,%d,%d,%d", &drone_x, &drone_y, &max_x, &max_y);
        // Create the window
        if (create_window(max_x, max_y) == 1)
        {
            getmaxyx(stdscr, max_y, max_x);
            // Rewrite the maximum values because windows size change (==1)
            sprintf(ptr_pos, "%d,%d,%d,%d", drone_x, drone_y, max_x, max_y);
        }
        // Draw the drone on the screen based on the obtained positions.
        draw_drone(drone_x, drone_y);
        // Call the function that obtains the key that has been pressed.
        handle_input(ptr_key, sem_key);
        /* Update drone position */
        //sem_wait(semaphorePos);
        //sscanf(ptr_pos, "%d,%d,%d,%d", &drone_x, &drone_y, &max_x, &max_y); // Obtain the values of X,Y from shared memory
        //sem_post(semaphorePos);
        //usleep(20000);
        //continue;
    }
    // Clean up and finish up resources taken by ncurses
    endwin(); 
    // close shared memories
    close(shm_key_fd);
    close(shm_pos_fd);

    // Close and unlink the semaphore
    sem_close(sem_key);
    sem_close(sem_pos);

    return 0;
}

int create_window(int max_x, int max_y)
{
    // Clear the screen
    clear();

    // Get the dimensions of the terminal window
    int new_max_x, new_max_y;
    getmaxyx(stdscr, new_max_y, new_max_x);

    // Draw a rectangular border using the box function
    box(stdscr, 0, 0);

    // Print a title in the top center part of the window
    mvprintw(0, (new_max_x - 11) / 2, "Drone Control");

    // Refresh the screen to apply changes
    refresh();
    if(new_max_x != max_x || new_max_y != max_y){return 1;}
    else{return 0;}
}

void draw_drone(int drone_x, int drone_y)
{

    // Draw a plus sign to represent the drone
    mvaddch(drone_y, drone_x, '+' | COLOR_PAIR(1));

    refresh();
}

void handle_input(int *shared_key, sem_t *semaphore)
{
    int ch;
    noecho(); // Disable echoing: no key character will be shown when pressed.
    if ((ch = getch()) != ERR)
    {
        // Store the pressed key in shared memory
        *shared_key = ch;
        // Signal the semaphore
        *shared_key = ch;    // Store the pressed key in shared memory
        sem_post(semaphore);    // Signal the semaphore to process key_manager.c
    }
    echo(); // Re-enable echoing
    flushinp(); // Clear the input buffer
}