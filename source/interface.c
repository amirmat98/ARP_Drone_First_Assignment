#include "interface.h"
#include "constants.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>


int main()
{
    void *ptr_key;        // Shared memory for Key pressing
    void *ptr_pos;        // Shared memory for Drone Position      
    sem_t *sem_key;       // Semaphore for key presses
    sem_t *sem_pos;       // Semaphore for drone positions


    ptr_key = access_shm_rdwr(SHAREMEMORY_KEY_1);
    ptr_pos = access_shm_rdwr(SHAREMEMORY_POSITION);

    sem_key = sem_open(SEMAPHORE_KEY_1, 0);
    sem_pos = sem_open(SEMAPHORE_POSITION, 0);

    // Initial drone position (middle of the blackboard)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;

    initscr();
    timeout(0); // Set non-blocking getch
    curs_set(0); // Hide the cursor from the terminal
    noecho(); // Disable echoing

    draw_window();
    refresh(); // Refresh the screen to apply changes

    // Write initial drone position in its corresponding shared memory
    int drone_position[2];
    drone_position[0] = drone_x;
    drone_position[1] = drone_y;
    set_drone_pos((int*)ptr_pos, drone_position);

    //sprintf(shared_position, "%d,%d", drone_x, drone_y);

    // Initialize color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // Drone
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Obstacle
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Target


    while (1) 
    {
        draw_window();
        // TODO: For all objects in objects
        draw_object(drone_position[0], drone_position[1]);
        // When all the objects are drawn
        refresh();

        handle_input(shared_memory, sem_pos);

        //Update drone position
        //sem_wait(semaphorePos);
        drone_position[0] = shared_position[0];
        drone_position[1] = shared_position[1];
        //sem_post(semaphorePos);

        //TODO: CHANGE TO NANOSLEEP
        usleep(REFRESH_RATE);
    }
    endwin();

    if (shm_unlink(SHAREMEMORY_KEY_1) == 1)
    {
        printf("Error removing %s\n", SHAREMEMORY_KEY_1);
        exit(1);
    }
        if (shm_unlink(SHAREMEMORY_POSITION) == 1)
    {
        printf("Error removing %s\n", SHAREMEMORY_POSITION);
        exit(1);
    }
    sem_close(sem_key);
    sem_unlink(SEMAPHORE_KEY_1);
    sem_close(sem_pos);
    sem_unlink(SEMAPHORE_POSITION);

    return 0;

}

void draw_window()
{
    clear();
    // Get the dimensions of the terminal window
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw a rectangular border using the box function
    box(stdscr, 0, 0);

    // Print a title in the center of the blackboard
    mvprintw(0, (max_x - 11) / 2, "Drone Control");
}

void draw_object(struct Object *ob)
{
    switch (ob->type)
    {
    case DRONE:
        mvaddch(ob->pos_y, ob->pos_x, '+' | COLOR_PAIR(1));
        break;
    case OBSTACLE:
        mvaddch(ob->pos_y, ob->pos_x, '1' | COLOR_PAIR(2));
        break;
    case TARGET:
        mvaddch(ob->pos_y, ob->pos_x, ob->sym | COLOR_PAIR(3));
        break;
    }
}

void handle_input(int *shared_key, sem_t *semaphore)
{
    int ch = getch();
    if (ch == -1)
    {
        perror("Error in getch()!");
    }
    else
    {
        // Store the pressed key in shared memory
        *shared_key = ch;
        // Signal the semaphore to notify the server
        sem_post(semaphore);
    }
    // Clear the input buffer
    flushinp();
}

void set_drone_pos(int *shared_pos, int drone_position[2]) 
{

    //sem_wait(semaphorePos);
    shared_pos[0] = drone_position[0];
    shared_pos[1] = drone_position[1];
    //sem_post(semaphorePos);
}

void *access_shm_rdonly(char *name)
{
    const int SIZE = 4096;
    /* get semaphores and shared memories */
    int shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == 1)
    {
        printf("Shared memory segment failed\n");
        exit(1);
    }
    
    // Memory-map the shared memory segment with read permissions
    void *ptr_key = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr_key == MAP_FAILED)
    {
        printf("Map failed\n");
        return 1;
    }
    return ptr_key;
}

void *access_shm_rdwr(char *name)
{
    const int SIZE = 4096;
    /* get semaphores and shared memories */
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == 1)
    {
        printf("Shared memory segment failed\n");
        exit(1);
    }
    
    // Memory-map the shared memory segment with read-write permissions
    void *ptr_key = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr_key == MAP_FAILED)
    {
        printf("Map failed\n");
        return 1;
    }
    return ptr_key;
}
