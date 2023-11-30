#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define MASS 1.0    // Mass of the object
#define DAMPING 0.1 // Damping coefficient
#define TIME_INTERVAL 0.1 // Time interval in seconds

// Functions
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