#ifndef CONSTANTS_H
#define CONSTANTS_H

// Shared memory key for key presses
#define SHAREMEMORY_KEY_1 "/shared_key"
#define SEMAPHORE_KEY_1 "/sem_key"

// Shared memory key for drone positions
#define SHAREMEMORY_POSITION "/shared_pos"
#define SEMAPHORE_POSITION "/sem_pos"

#define SHAREMEMORY_KEY_2 5678
#define SEMAPHORE_KEY_2 "/my_semaphore_2"

// Shared memory key for drone control (actions).
//#define SHAREMEMORY_KEY_3 3
//#define SEMAPHORE_KEY_3 "/my_semaphore_3"

// Special value to indicate no key pressed
#define NO_KEY_PRESSED 0

#define DELAY 50000

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

#endif // CONSTANTS_H