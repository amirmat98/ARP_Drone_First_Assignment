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


//#define REFRESH_RATE 20000

void create_window();
void draw_drone(int drone_x, int drone_y);
void handle_input(int *shared_key, sem_t *semaphore);


#endif //INTERFACE_H