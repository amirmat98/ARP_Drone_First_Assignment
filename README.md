# Description
[Università degli studi di Genova](https://unige.it/en/ "University of Genova")

Professor: [Renato Zaccaria](renato.zaccaria@unige.it "Renato Zaccaria")

Student: [AmirMahdi Matin](https://github.com/amirmat98 "AmirMahdi Matin")  - 5884715 - Robotics Engineering

# Table of Contents


# ARP First Assignment - 20231202
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
TThis assignment constitutes the initial segment of the project for the Advanced and Robot Programming course at UniGe during the winter semester of 2023/2024.
The project was completed by AMIRMAHDI MATIN with the student ID 5884715.

## Installation & Usage
We utilized the `Makefile` for configuring the project.

To build executables, simply use the following command:

```
make
```
in the project directory.

To run the game hit:
```
make run
```
Upon occurrence of this event, five separate windows containing konsole processes will be initiated, with each window dedicated to a distinct section.

To remove the executables simply hit 
```
make clean
```

### Operational instructions, controls
To control the drone, utilize the following keybindings:

    `q` `w` `e`       `u` `i` `o`
    `a` `s` `d`   or  `j` `k` `l`
    `z` `x` `c`       `m` `,` `.`

## Overview 
The initial segment assumes the initial six components:
- Main
- Server (blackboard using Posix Shared Memory)
- Window (interface)
- Watchdog
- Drone
- Key Manager

All of the aforementioned components were fabricated. To obtain more information, please consult *SECTION 1 AND 2*.


Provide concise explanations of all active components, including their functions, the primitives they utilize, and the algorithms involved.

### Main
The main process is the ultimate progenitor of all processes. The software generates child processes through the use of the `fork()` function and executes them within a wrapper program called `Konsole`. This allows for the display of the current status and debugging messages until an additional thread/process is created specifically for collecting log messages.

Upon generating offspring, the process enters an infinite while loop, persistently awaiting the termination of all processes. Once this condition is met, the process proceeds to terminate itself.


### Server
The server process is the essential component of this project. This process generates all the components of the shared memory, including the segments and semaphores. It is also responsible for tidying them up when they are interrupted.

The server initializes semaphores and shared memory segments, and then starts an endless while loop, waiting for a signal from the watchdog. Upon exiting the loop (which is not expected to occur, as the current assumption is that the server only closes when stopped), it performs a cleanup of the segments and semaphores, ensuring that no remnants are left behind. 



### Watchdog
The role of a watchdog is to oversee the operational status of all processes, ensuring that they are currently active and have not been terminated.

During initialization, the PIDs of processes are retrieved from a designated shared memory segment. It is important to note that in the main process, which is running the wrapper process Konsole, it is not feasible to obtain the true PID using `fork()` in `main` at this stage.

Subsequently, the process enters a while loop, wherein it dispatches a `SIGUSR1` signal to all the processes to verify their liveness. They reply to the watchdog by sending a `SIGUSR2` signal, using the process ID obtained from `siginfo_t`. This resets the counter for program responses. When the counter for any of them hits the threshold, Watchdog triggers a `SIGINT` signal to all processes and terminates, ensuring the closure of all semaphores it was utilizing. A similar occurrence arises when Watchdog is disrupted.


### Interface (Window)

### Drone

### Key manager

### Additional Comments
- The watchdog obtains the process ID (PID) of all the processes through a shared memory section. For this procedure to be executed, three semaphores are necessary. (In the view of the writer, two would suffice, but currently we needed to secure the stability of the logical progression and did not have the opportunity to refine this solution.)

