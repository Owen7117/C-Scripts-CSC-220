// Owen O'Neil
// Program 2
// 2/18/2025
// This program simulates the processes switching mechanism in an operating system


// import all necessary directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create the process struct
struct process {
    // set all the objects in the struct
    int pid;
    char processName[30];
    int priority;
    int runtime;
    int remainingTime;
    int dispatcher;

};
// create the struct array globally
struct process list[20];
// create the dispatcher array with set elements
struct process dispatcher = {6787, "DISPATCHER", 0, 4, 4};

// call all subroutines created below the main subroutine so that they can be accessed
int generate_pid();
int what_goes_first();
int all_processes_running();

// main subroutine
int main() {
    // counter for number of processes
    int count = 0;

    // while the process name is not Q, gather the inputs
    while (1) {
        printf("Enter The Process Details:");

        // call the tempname and give it a length
        char tempName[30];
        // call the temporary priority and runtime from the struct
        int tempPriority, tempRuntime;

        // Read the process name first to check for 'Q'
        scanf(" %s", tempName);

        // If the user enters 'Q', break out of the loop
        if (strcmp(tempName, "Q") == 0) {
            break;
        }

        // Read the priority and runtime only if it's not 'Q'
        if (scanf(" %d %d", &tempPriority, &tempRuntime) != 2) {
            printf("Invalid input format. Please enter again.\n");
            continue;
        }

        // Assign values to the list
        list[count].pid = generate_pid(count);
        strcpy(list[count].processName, tempName);
        list[count].priority = tempPriority;
        list[count].runtime = tempRuntime;
        // also set the remaining time to be the same as runtime
        list[count].remainingTime = tempRuntime;
        // increment process count
        count++;
    }
    // print the categories
    printf("\n%-15s %-10s %-10s %-15s\n", "Process Name", "Pid", "Priority", "RemainingTime");

    // while at least one process is still running
    while (all_processes_running(count)) {
        // determine the next process
        int highest_pid = what_goes_first(count);

        // no processes left
        if (highest_pid == -1) {
            break;
        }

        for (int j = 0; j < count; j++) {
            // next up priority
            if (list[j].pid == highest_pid) {
                int c = 0;
                while (c < 5 && list[j].remainingTime > 0) {
                    // print the process name, pid, priority, and remaining time
                    printf("%-15s %-10d %-10d %-15d\n", list[j].processName, list[j].pid, list[j].priority, list[j].remainingTime - 1);
                    // decrement remaining time
                    list[j].remainingTime--;
                    // increment the c count by 1
                    c++;
                }

                // Dispatcher runs after the process execution
                for (int i = 0; i < 4; i++) {
                    printf("%-15s %-10d %-10d %-15d\n", dispatcher.processName, dispatcher.pid, dispatcher.priority, 3 - i);
                }

                // Increase priority of remaining processes
                for (int k = 0; k < count; k++) {
                    if (list[k].priority > 0) {
                        list[k].priority--;
                    }
                }
            }
        }
    }
    // return 0 because the subroutine is an int
    return 0;
}

// Function to pick the process with the highest priority
int what_goes_first(int count) {
    // create current task adn set it
    int current_task = -1;

    for (int i = 0; i < count; i++) {
        // if the remaining time is greater than 0 and if the current task is -1 or the priority is lower than the current task priority, then set that as the new current task
        if (list[i].remainingTime > 0) {
            if (current_task == -1 || list[i].priority < list[current_task].priority) {
                current_task = i;
            }
        }
    }
    //return the current tasks pid or -1 if none left
    if (current_task == -1) {
        return -1;
    }
    return list[current_task].pid;
    }

// check if pid is taken
int is_pid_taken(int pid, int count) {
    for (int i = 0; i < count; i++) {
        // if the pid is already taken return 1
        if (list[i].pid == pid) {
            return 1;
        }
    }
    // otherwise return 0
    return 0;
}

// generate the pid
int generate_pid(int count) {
    // create the random id variable
    int rand_id;
    // while the pid isn't taken get a random number from 1000-4000
    do {
        rand_id = rand() % 3000 + 1000;
    } while (is_pid_taken(rand_id, count));
    // return the number
    return rand_id;
}
// see if all processes have a remaining run time
int all_processes_running(int count) {
    for (int i = 0; i < count; i++) {
        // if at least one process is running return 1
        if (list[i].remainingTime > 0) {
            return 1;
        }
    }
    // otherwise all processes have finished and return 0
    return 0;
}
