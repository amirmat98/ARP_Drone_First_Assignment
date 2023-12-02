#ifndef KEY_MANAGER_H    
#define KEY_MANAGER_H


/*
    Determine what action should be performed from user input
    @param pressed_key 
    @returns Message with with pressed key
*/
char* determine_action(int pressed_key);

/*
    Sends action to Drone
    @param action translated action
*/
void send_action_to_drone(char* action);

/*
    Clears shared Memory 
    @param shared_memory memory to be cleaned
*/
void clear_shared_memory(int* shared_memory);


#endif // KEY_MANAGER_H
