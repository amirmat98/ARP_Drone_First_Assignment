#include "drone.h"
#include "constants.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



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


    // Variable declaration segment

    double x = shared_position[0]; // Initial position of x. OBTAINED FROM INTERFACE.C
    double v_x = 0.0;    // Initial velocity of x
    double force_x = 0.1; // Applied force in the x direction

    double y = shared_position[1];   // Initial position of y. OBTAINED FROM INTERFACE.C
    double v_y = 0.0;    // Initial velocity of y
    double force_y = 0.1; // Applied force in the y direction    

    // Simulate the motion in an infinite loop using Euler's method
    while (1) {
        euler_method(&x, &v_x, force_x, &y, &v_y, force_y);
        printf("X - Position: %.2f / Velocity: %.2f\t|\t", x, v_x);
        printf("Y - Position: %.2f / Velocity: %.2f\n", y, v_y);

        // Write new drone position to shared memory
        shared_position[0] = x;
        shared_position[1] = y;

        // Introduce a delay to simulate real-time intervals
        usleep(TIME_INTERVAL * 1e6);
    }

    // Detach the shared memory segment
    shmdt(sharedPosition);

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