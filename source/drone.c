#include "drone.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>



int main() {

    // Initialize shared memory for drone positions
    int shared_pos;
    int *shared_position;

    if ((shared_pos = shmget(SHAREMEMORY_KEY_2, 2 * sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shared_position = shmat(shared_pos, NULL, 0)) == (int *)-1) {
        perror("shmat");
        exit(1);
    }


    // Initialize shared memory for drone actions.
    int shared_act;
    char *shared_action;

    if ((shared_act = shmget(SHAREMEMORY_KEY_3, 80*sizeof(char), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        //exit(1);
    }

    if ((shared_action = shmat(shared_act, NULL, 0)) == (char *)-1) {
        perror("shmat");
        //exit(1);
    }

    sem_t *semaphore_act = sem_open(SEMAPHORE_KEY_3, O_CREAT, 0666, 0);
    if (semaphore_act == SEM_FAILED)
    {
        perror("sem_open");
        //exit(1);
    }

    // Variable declaration segment
    int x; int y;
    int action_x; int action_y;
    sleep(1);
    sscanf(shared_position, "%d,%d", &x, &y); // Obtain the values of x,y from shared memory
    //x = (float)x;
    //y = (float)y;

    double v_x = 0.0;    // Initial velocity of x
    double force_x = 1.0; // Applied force in the x direction

    double v_y = 0.0;    // Initial velocity of y
    double force_y = 1.0; // Applied force in the y direction


    // Simulate the motion in an infinite loop using Euler's method
    while (1) {
        //eulerMethod(&x, &v_x, force_x, &y, &v_y, force_y);
        //printf("x - Position: %.2f / Velocity: %.2f\t|\t", x, v_x);
        //printf("y - Position: %.2f / Velocity: %.2f\n", y, v_y);
        
        sscanf(shared_action, "%d,%d", &action_x, &action_y);
        printf("Action (x,y): %s\n",shared_action);

        step_method(&x,&y,action_x,action_y,shared_action);
        printf("x - Position: %d / Velocity: %.2f\t|\t", x, v_x);
        printf("y - Position: %d / Velocity: %.2f\n", y, v_y);

        // Write new drone position to shared memory
        sprintf(shared_position, "%d,%d", x, y);

        // Introduce a delay to simulate real-time intervals
        usleep(TIME_INTERVAL * 1e6);
    }

    // Detach the shared memory segment
    shmdt(shared_position);
    shmdt(shared_action);

    return 0;
}


// Implementation of the eulerMethod function
void euler_method(double *x, double *v_x, double force_x, double *y, double *v_y, double force_y) {
    double acceleration_x = (force_x - DAMPING * (*v_x)) / MASS;
    double acceleration_y = (force_y - DAMPING * (*v_y)) / MASS;

    // Update velocity and position for x using Euler's method
    *v_x = *v_x + acceleration_x * TIME_INTERVAL;
    *x = *x + (*v_x) * TIME_INTERVAL;

    // Update velocity and position for y using Euler's method
    *v_y = *v_y + acceleration_y * TIME_INTERVAL;
    *y = *y + (*v_y) * TIME_INTERVAL;
}

// Moving the drone step by step as initial development
void step_method(int *x, int *y, int action_x, int action_y, char *shared_action){
    (*x) = (*x) + action_x;
    (*y) = (*y) + action_y;

    sprintf(shared_action, "%d,%d", 0, 0); // Zeros written on action memory
}