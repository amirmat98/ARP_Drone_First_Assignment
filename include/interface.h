#ifndef INTERFACE_H
#define INTERFACE_H   

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>


void create_blackboard();
void draw_drone(int drone_x, int drone_y);
void handle_input(int *shared_key, sem_t *semaphore);
void initialize_drone_position(int *shared_pos, int drone_position[2]);


#endif //INTERFACE_H