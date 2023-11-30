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


int main() {
    double x = 100.0;  // Initial position of x
    double v_x = 0.0;    // Initial velocity of x
    double force_x = 10.0; // Applied force in the x direction

    double y = 50.0;   // Initial position of y
    double v_y = 0.0;    // Initial velocity of y
    double force_y = 5.0; // Applied force in the y direction

    // Simulate the motion in an infinite loop using Euler's method
    while (1) {
        euler_method(&x, &v_x, force_x, &y, &v_y, force_y);
        printf("X - Position: %.2f / Velocity: %.2f\t|\t", x, v_x);
        printf("Y - Position: %.2f / Velocity: %.2f\n", y, v_y);

        // Introduce a delay to simulate real-time intervals
        usleep(TIME_INTERVAL * 1e6);
    }

    return 0;
}