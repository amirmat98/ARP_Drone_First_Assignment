#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void handle_input(){
    int ch;

    while ( (ch = getch() ) != ERR) {
        // Print the pressed key to the standard output
        printf("Pressed key: %c\n", ch);
    }

    // Clear the input buffer
    flushinp();
}

int main(){
    initscr();
    timeout(0); // Set non-blocking getch
    createBlackboard();

    // Initial drone position (middle of the blackboard)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int drone_x = max_x / 2;
    int drone_y = max_y / 2;

    // Initialize color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);


    while(1){
        draw_drone(drone_x, drone_y);
        handle_input();
        usleep(100000); // Add a small delay to control the speed
        continue;
    }

    endwin();
    return 0;

}