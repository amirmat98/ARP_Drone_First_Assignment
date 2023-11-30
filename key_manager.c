#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

// Shared memory key
#define SHAREMEMORY_KEY 1234

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
