#ifndef CONSTANTS_H
#define CONSTANTS_H

// Shared memory key for key presses
#define SHAREMEMORY_KEY "/shared_keyboard"
#define SHAREMEMORY_KEY_1 1
#define SEMAPHORE_KEY "/my_semaphore_1"

// Shared memory key for drone positions
#define SHAREMEMORY_POSITION "/shared_drone_pos"
#define SHAREMEMORY_KEY_2 2
#define SEMAPHORE_POSITION "/my_semaphore_2"


// Shared memory key for drone control (actions).
#define SHAREMEMORY_ACTION "/shared_control"
#define SHAREMEMORY_KEY_3 3
#define SEMAPHORE_ACTION "/my_semaphore_3"

// Special value to indicate no key pressed
#define NO_KEY_PRESSED 0

//#define DELAY 50000

/*
enum objects
{
    DRONE,
    TARGET,
    OBSTACLE,
    NONE
};

struct Object 
{   
  enum objects type;
  int pos_x;          
  int pos_y;
  char sym;       
};
*/

#endif // CONSTANTS_H