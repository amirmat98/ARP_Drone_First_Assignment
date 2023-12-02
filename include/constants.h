#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FLOAT_TOLERANCE 0.0044

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


#endif // CONSTANTS_H