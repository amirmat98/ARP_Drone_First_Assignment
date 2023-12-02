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
    void *ptr_key;        // Shared memory for Key pressing
    void *ptr_pos;        // Shared memory for Drone Position      
    void *ptr_action;     // Shared memory ptr for actions
    sem_t *sem_key;       // Semaphore for key presses
    sem_t *sem_pos;       // Semaphore for drone positions
    sem_t *sem_action;    // Semaphore for actions

    // Shared memory for KEY PRESSING
    ptr_key = create_shm(SHAREMEMORY_KEY);
    sem_key = sem_open(SEMAPHORE_KEY, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem_key == SEM_FAILED) {
        perror("sem_key failed");
        exit(1);
    }
    // Shared memory for DRONE POSITION
    ptr_pos = create_shm(SHAREMEMORY_POSITION);
    sem_pos = sem_open(SEMAPHORE_POSITION, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem_pos == SEM_FAILED) {
        perror("sem_pos failed");
        exit(1);
    }
    // Shared memory for DRONE ACTION
    ptr_action = create_shm(SHAREMEMORY_ACTION);
    sem_action = sem_open(SEMAPHORE_ACTION, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (sem_pos == SEM_FAILED) {
        perror("sem_pos failed");
        exit(1);
    }    
    
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
        handle_input((int*) ptr_key, sem_key);
        /* Update drone position */
        //sem_wait(semaphorePos);
        sscanf((char*) ptr_pos, "%d,%d", &drone_x, &drone_y); // Obtain the values of X,Y from shared memory
        //sem_post(semaphorePos);
        usleep(20000);
        continue;
    }
    endwin();

    // Detach the shared memory segments
    shmdt(SHAREMEMORY_KEY);
    shmdt(SHAREMEMORY_POSITION);
    
    // Close and unlink the semaphores
    sem_close(sem_key);
    sem_close(sem_pos);
    sem_close(sem_action);
    sem_unlink(SEMAPHORE_KEY);
    sem_unlink(SEMAPHORE_POSITION);
    sem_unlink(SEMAPHORE_ACTION);
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

void *create_shm(char *name)
{
    const int SIZE = 4096;  // the size (in bytes) of shared memory object

    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open of shm_key");
        exit(1);
    }
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    void *shm_ptr = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0); 
    if (shm_ptr == MAP_FAILED)
    {
        perror("Map Failed");
        exit(1);
    }
    return shm_ptr;
}