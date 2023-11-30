#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Shared memory key
#define SHAREMEMORY_KEY 1234

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

void handle_input(int *shared_key){
    int ch;

    if ( (ch = getch() ) != ERR) {
        // Debugging: Print the pressed key
        printf("Pressed key: %d\n", ch);

        // Store the pressed key in shared memory
        *shared_key = ch;
    }

    // Clear the input buffer
    flushinp();
}

int main(){
    initscr();
    timeout(0); // Set non-blocking getch
    create_blackboard();

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

    // Initial drone position (middle of the blackboard)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;

    // Initialize color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);


    while(1){
        create_blackboard();
        draw_drone(drone_x, drone_y);
        handle_input(shared_memory);
        usleep(100000); // Add a small delay to control the speed
        continue;
    }

    // Detach the shared memory segment
    //shmdt(shared_memory);

    shmctl(shared_key, IPC_RMID, NULL);
    endwin();
    return 0;

}