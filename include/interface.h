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

int create_window(int max_x, int max_y);
void draw_drone(int drone_x, int drone_y);
void handle_input(int *shared_key, sem_t *semaphore);


#endif //INTERFACE_H