// Owen O'Neil
// Program 3 Mario
// 3/4/2025

// import all necessary directives
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

// set the semaphore as s
sem_t s;
// create an array with the thread names
char threadnames[6][20] = {"MARIO","LUIGI","BOWSER","PRINCESSPEACH","TOAD","YOSHI"};
// create an array with the valve numbers
int valves[35] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
// create an array for all the corresponding complexities
int complex_arr[35] = {};
// keep a count on how may valves are open
int valves_opened = 0;
int total_valves = 35;


// subroutine that gets a random complexity for each valve
int complexity_num() {
    int rand_complex;
    // for every valve index give it a random num form 2-10
    for(int i=0; i < 35; i++){
        rand_complex = rand() % 9 + 2;
        // add it to the array
        complex_arr[i] = rand_complex;
    }
    // return 0 since it is an int subroutine
    return 0;
}


// subroutine that opens the valves using threads
void *pipes(void *arg) {
    // set the thread names as a variable
    char *name = (char *)arg;
    // while there are valves not opened
    while (1) {
        // call the semaphore wait so that s is now 0 to stop multiple threads from opening the same pipe
        sem_wait(&s);

        // check if all valves are open
        if (valves_opened >= total_valves) {
            sem_post(&s);
            // exit the loop if all valves are opened
            break;
        }

        // get a random valve
        int rand_valve;
        do {
            // pick a random index
            rand_valve = rand() % 35;
            // repeat if valve is already opened
        } while (valves[rand_valve] == -1);

        // Get the valve number
        int valve_num = valves[rand_valve];
        // set the time remaining using the complex number array
        int time_remaining = complex_arr[rand_valve];

        // mark valve as opened
        valves[rand_valve] = -1;
        // increment the open valves count by 1
        valves_opened++;
        // allow other threads to get valves now that the current valve has been selected and removed from the list
        // (end of critical section)
        // use sem_post to change s back to 1 to allow another thread to select a valve
        sem_post(&s);

        // simulate valve opening
        // while there is still time remaining to open the valve
        while (time_remaining > 0) {
            printf("%s OPENING VALVE %03d - TIME REMAINING %d\n", name, valve_num, time_remaining);
            // print in real time
            fflush(stdout);
            // wait ine second to simulate the time it takes to open in seconds
            sleep(1);
            // decrement the time remaining by 1
            time_remaining--;
        }
        // when the valve hase no time remaining, then print that the valve is opened
        printf("VALVE %03d OPENED BY %s\n", valve_num, name);
        // print in real time
        fflush(stdout);
    }
    // return numm because of void subroutine
    return NULL;
}


// main subroutine
int main(){
    // set the srand function so that every rand() number is actually random
    srand(time(0));
    // get the random complexity for each valve
    complexity_num();
    printf("Valves created:\n");
    for(int i=1; i <= 35; i++) {
        printf("VALVE %03d - %d\n", i, complex_arr[i-1]);
    }
    printf("Characters hired:\n");
    for(int j=0; j < 6; j++) {
        printf("%s\n", threadnames[j]);
    }
    // initiate the semaphore with a value of 1
    sem_init(&s,0, 1);
    printf("MUSHROOM KINGDOM, HERE WE COME!!!\n");
    // initiate the threads
    pthread_t threads[6];
    // create 6 threads for the 6 characters while passing them through the pipes subroutine
    for (int i = 0; i < 6; i++) {
        pthread_create(&threads[i], NULL, pipes, threadnames[i]);
    }

    // wait for all threads to complete
    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }
    // use sem_destroy to end the semaphore
    sem_destroy(&s);
    // return 0 because of int subroutine
    return 0;
}


