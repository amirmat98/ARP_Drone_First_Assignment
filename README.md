ARP First Assignment 20231202
# Assignment details
Deadline: December 3, 2023, 12:59 pm.

After this date you can upload your work, but a penalty will be applied in the evaluation mark. 

The penalty is a percentage that monotonically increases from 0% up to 50% at the date of the next assignment's deadline.

 Follow these rules punctually and literally: no mercy for those who violate them 😈

All group member(s) upload only one archive file
No executables allowed
Update the groups list in the shared file so that the assignment can be put into relation with its authors.
Your work shall include (be both synthetic and complete):

sketch of the architecture
short definitions of all active components : what they do, which primitives used, which algorithms)
list of components, directories, files
makefile
instructions for installing and running
operational instructions
whatever else you think useful
What you upload cannot be substituted for no reason.

After submission, public your work on GITHUB.

# Assignment 1
This assignment represents the first part of the project for Advanced and Robot Programming course on UniGe in the winter semester 2023/2024.

The project have been done by AMIRMAHDI MATIN 5888447

## Installation & Usage
For project's configuration we have used `Makefile`.

To build executables simply hit:
```
make
```
in the project directory.

To run the game hit:
```
make run
```
when this happens a 5 windows with konsole processes will launch, each one for different segment.

To remove the executables simply hit 
```
make clean
```

### Operational instructions, controls
To operate the drone use following keybindings
    `q` `w` `e`       `u` `i` `o`
    `a` `s` `d`   or  `j` `k` `l`
    `z` `x` `c`       `m` `,` `.`

The given bindings represent *josue*

## Overview 
The first part assumes first 6 components:
- Main
- Server (blackboard using Posix Shared Memory)
- Window (User interface)
- Watchdog
- Drone
- Keyboard Manager

All of above mentioned components were created. For further details please refer to *SECTION 1 AND 2*.


General overview of first assignment, tasks, what was accomplished

short definitions of all active components : what they do, which primitives used, which algorithms)

### Main
Main process is the father of all processes. It creates child processes by using `fork()` and runs them inside a wrapper program `Konsole` to display the current status, debugging messages until an additional thread/process for colleceting log messages.

After creating children, process stays in a infinte while loop awaiting termination of all processes, and when that happens - terminate itself.

### Server
Server process is the heart of this project. It creates all segments of the shared memory, and semaphores. It is also responsible of cleaning up of them, when being interrupted.

Server operates by creating the semaphores and shared memory segments, then enters an infinite while loop awaiting for a signal from watchdog. After getting out of the loop (which should never happen, at the moment we assume the server closes only when interrupted) it cleans up the segments and semaphores, making sure no artifacts are left behind. 


### Watchdog
Watchdog's job is to monitor the "health" of all of the processes, which means at this point if processes are running and not closed.

During initialization it gets from special shared memory segment the PIDs of processes, (remember that in main we are running the wrapper process Konsole, so its not possible to get to know the actual PID from `fork()` in `main`, at this point at least).

Then the process enters while loop where it sends `SIGUSR1` to all of the processes checking if they are alive. They respond with `SIGUSR2` back to watchdog, knowing its PID thanks to `siginfo_t`. This zeroes the counter for programs to response. If the counter for any of them reaches the threshold, Watchdog sends `SIGINT` to all of the processes, and exits, making sure all of the semaphores it was using are closed. The same thing happens when Watchdog is interrupted.

### Interface (Window)
foo *josue*
### Drone
foo *josue*
### Keyboard manager

### Additional Comments
- Watchdog gets the PID from all of the process through a shared memory segment. This operation requires 3 semaphores (In author's opinion 2 would be enough, yet at this moment we needed tu ensure the stability of logic flow, and did not have time to polish this solution.)

- We assume processes to be run in order it happens in `main.c`. If someone wishes to run processes seperatly, they may encounter segfaults that happen due to the fact that processes wish to access a non-existing shared memory components. Therefore we suggest to run project the way it is described in *Installation section*

