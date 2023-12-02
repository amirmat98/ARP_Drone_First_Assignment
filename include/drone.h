#ifndef DRONE_H
#define DRONE_H 

#define MASS 1.0    // Mass of the object
#define DAMPING 0.1 // Damping coefficient
#define TIME_INTERVAL 0.1 // Time interval in seconds

void euler_method(double *x, double *v_x, double force_x, double *y, double *v_y, double force_y, double *max_x, double *max_y);
void step_method(int *x, int *y, int action_x, int action_y);

#endif //DRONE_H