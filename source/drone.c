#include "drone.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <math.h>



int main() 
{
    // Initialize shared memory for drone positions
    int shared_pos;
    char *shared_position;
    // Initialize shared memory for drone actions.
    int shared_act;
    char *shared_action;

    // Shared memory for DRONE POSITION
    shared_pos = shm_open(SHAREMEMORY_POSITION, O_RDWR, 0666);
    shared_position = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shared_pos, 0);

    // Shared memory for DRONE CONTROL - ACTION
    shared_act = shm_open(SHAREMEMORY_ACTION, O_RDWR, 0666);
    shared_action = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shared_act, 0);


    sem_t *semaphore_act = sem_open(SHAREMEMORY_ACTION, O_CREAT, 0666, 0);
    if (semaphore_act == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    // Variable declaration segment
    usleep(100000); // To let the interface.c process write the initial positions first.
    int x; int y;
    int max_x; int max_y;
    int action_x; int action_y;
    //sscanf(shared_position, "%d,%d", &x, &y); // Obtain the values of x,y from shared memory
    //printf("%d,%d",x,y);
    sscanf(shared_position, "%d,%d,%d,%d", &x, &y, &max_x, &max_y); // Obtain the values of X,Y from shared memory

    // Variables for euler method

    double pos_x = (double)x;
    double v_x = 0.0;    // Initial velocity of x
    double force_x = 1.0; // Applied force in the x direction

    double pos_y = (double)y;
    double v_y = 0.0;    // Initial velocity of y
    double force_y = 1.0; // Applied force in the y direction

    bool euler_method_flag = true; // Obtain from keyboard manager

    // Simulate the motion in an infinite loop using Euler's method
    while (1) 
    {
        int x_i;
        int y_i;
        sscanf(shared_position, "%d,%d,%d,%d", &x_i, &y_i, &max_x, &max_y);
        sscanf(shared_action, "%d,%d", &action_x, &action_y);
        printf("Action (x,y): %s\n",shared_action);

        /* DRONE CONTROL WITH THE DYNAMICS FORMULA*/
        if(euler_method)
        {
            // Only values between -1 to 1 are used to move the drone
            if(action_x >= -1.0 && action_x <= 1.0)
            {
                force_x += (double)action_x;
                force_y += (double)action_y;
                // Capping the force to a maximum value
                double force_max = 20;
                if(force_x>force_max){force_x = force_max;}
                if(force_y>force_max){force_y = force_max;}
                if(force_x<-force_max){force_x = -force_max;}
                if(force_y<-force_max){force_y = -force_max;}
            }
            // For now, other values for action represent a STOP command.
            else
            {
                force_x = 0.0; 
                force_y = 0.0;
            }
            // Calling the function
            double max_x_f = (double)max_x;
            double max_y_f = (double)max_y;
            euler_method(&pos_x, &v_x, force_x, &pos_y, &v_y, force_y, &max_x_f, &max_y_f);
            // Only print the positions when there is still velocity present.
            if(fabs(v_x) > FLOAT_TOLERANCE || fabs(v_y) > FLOAT_TOLERANCE){
                printf("Force (X,Y): %.2f,%.2f\n",force_x,force_y);
                printf("X - Position: %.2f / Velocity: %.2f\t|\t", pos_x, v_x);
                printf("Y - Position: %.2f / Velocity: %.2f\n", pos_y, v_y);
                fflush(stdout);
            }
            // Write zeros on action memory
            sprintf(shared_action, "%d,%d", 0, 0); 
            // Write new drone position to shared memory
            int x_f = (int)round(pos_x);
            int y_f = (int)round(pos_y);
            sprintf(shared_position, "%d,%d,%d,%d", x_f, y_f, max_x, max_y);
        }

        /* DRONE CONTROL WITH THE STEP METHOD*/
        else
        {
            if(action_x >= -1.0 && action_x <= 1.0)
            {
                // Calling the function
                step_method(&x,&y,action_x,action_y);
                // Only print when there is change in the position.
                if(action_x!=0 || action_y!=0)
                {
                    printf("Action (X,Y): %s\n",shared_action);
                    printf("X - Position: %d / Velocity: %.2f\t|\t", x, v_x);
                    printf("Y - Position: %d / Velocity: %.2f\n", y, v_y);
                    fflush(stdout);
                }
                sprintf(shared_action, "%d,%d", 0, 0); // Zeros written on action memory
                // Write new drone position to shared memory
                sprintf(shared_position, "%d,%d,%d,%d", x, y, max_x, max_y);
            }
        }

        // Introduce a delay on the loop to simulate real-time intervals.
        usleep(TIME_INTERVAL * 1e6);
    }

    // Detach the shared memory segment
    shmdt(shared_position);
    shmdt(shared_action);

    return 0;
}


// Implementation of the eulerMethod function
void euler_method(double *x, double *v_x, double force_x, double *y, double *v_y, double force_y, double *max_x, double *max_y) 
{
    double acceleration_x = (force_x - DAMPING * (*v_x)) / MASS;
    double acceleration_y = (force_y - DAMPING * (*v_y)) / MASS;

    // Update velocity and position for X using Euler's method
    *v_x = *v_x + acceleration_x * TIME_INTERVAL;
    *x = *x + (*v_x) * TIME_INTERVAL;
    if (*x < 0){*x = 0;}
    if (*x > *max_x){*x = *max_x-1;}

    // Update velocity and position for Y using Euler's method
    *v_y = *v_y + acceleration_y * TIME_INTERVAL;
    *y = *y + (*v_y) * TIME_INTERVAL;
    if (*y < 0){*y = 0;}
    if (*y > *max_y){*y = *max_y-1;}
}

// Moving the drone step by step as initial development
void step_method(int *x, int *y, int action_x, int action_y)
{
    (*x) = (*x) + action_x;
    (*y) = (*y) + action_y;


    //sprintf(shared_action, "%d,%d", 0, 0); // Zeros written on action memory
}