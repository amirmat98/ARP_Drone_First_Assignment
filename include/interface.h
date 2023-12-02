#ifndef INTERFACE_H
#define INTERFACE_H   

#include "constants.h"
#include <semaphore.h>


#define REFRESH_RATE 20000

/*
    Clears the screen, creates window with frames and refreshe and 
*/
void draw_window();

/*
    Draw object on the screen depending on object type @param ob pointer to struct Object
*/

void draw_object(struct Object *ob);

/*
    Reads the input from the keyboard and sends it to shared memory
    @param shared_key - Shared memory to write to
    @param semaphore - semaphore associated with that memory
*/

void handle_input(int *shared_key, sem_t *semaphore);

void set_drone_pos(int *shared_pos, int drone_position[2]);


void *access_shm(char *name);

#endif //INTERFACE_H