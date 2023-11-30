#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void summon(char** programArgs){
    execvp("konsole",programArgs);
    perror("Execution failed");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int child_exit_status;
    char* interface[] = {"konsole", "-e", "./build/interface", NULL};
    char* key_manager[] = {"konsole", "-e", "./build/key_manager", NULL};
    int number_process; //number of processes
    number_process = 2;

    for (int i = 0; i<number_process; i++){
        // Summoning processes as childs of the caller
        pid_t pid = fork();

        if (pid < 0){
            exit(EXIT_FAILURE);
        }

        else if(!pid){
            if (i == 0){
                summon(interface);
            }
            else if (i == 1){
                summon(key_manager);
            }
        }
        
        else{
            // Continue with father code
            printf("Summoned child with pid: %d\n", pid);
            fflush(stdout);
        }

    }


    for (int i = 0; i < number_process; i++) {
        //waits for childrens to end
        int finished_pid =  wait(&child_exit_status);
        //if the childs have exited normally print their pid and status
        if(WIFEXITED(child_exit_status)){
            printf("Child %d terminated with exit status: %d\n", finishedPid, WEXITSTATUS(childExitStatus));
        }
    }

    printf("Childs exited and FIFO closed. Father closing\n");

    return EXIT_SUCCESS;

}