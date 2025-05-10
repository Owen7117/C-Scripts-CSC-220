//Owen O'Neil
//Program 1: Chess
//1/30/2025

//import necessities
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// call all subroutines so that it can be used in main
void create_grid();
void print_grid();
bool check_queens();


// initialize the size of the grid
int size = 0;
char arr[8][8];

// creates the start and end variables for the timer
long time_diff_ns(struct timespec start, struct timespec end) {
    // (end.tv_sec - start.tv_sec) calculates the seconds and * 1000000000L + (end.tv_nsec - start.tv_nsec) calculates the time in seconds and nanoseconds
    return (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
}
// main subroutine
int main() {
    // use srand to be able to get a random num for later in the program
    srand(time(0));
    // initialize the user input
    char input[10];

    // Get array size from user
    printf("Enter the size of the board:");
    fgets(input, sizeof(input), stdin);
    // if the size is not the enter key then make size the same as the user input
    if (input[0] != '\n') {
        size = atoi(input);
    }
    // while the user input is between 3 and 10 then prompt the user to pick again
    while (size < 2 || size > 11) {
        printf("Invalid Input: Number must be 2-11\n");
        printf("Enter the size of the board (default is 8): ");
        fgets(input, sizeof(input), stdin);
        if (input[0] != '\n') {
            size = atoi(input);
        // make the default option 8
        } else {
            size = 8;  // Default to 8 if Enter is pressed again
        }
    }
    // stores time stamps for start time and current time
    struct timespec start_time, current_time;
    // start eth timer
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // while the grid has not meet all the requirements, create a new grid and check again
    do {
        clock_gettime(CLOCK_MONOTONIC, &current_time);  // Get current time
        // call the create_grid subroutine
        create_grid();
        // Check if 30 seconds elapsed
        if (current_time.tv_sec - start_time.tv_sec > 30) {
            // if it has been 30 seconds and no solution is found then end the program
            printf("RUN OUT OF TIME\n");
            return 1;
        }// call the check queens function
    } while (!check_queens());

    // once the grid is found, print it
    print_grid();
    // get the final time check for when the grid is found
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    // calculate the elapsed time in nanoseconds
    long elapsed_ns = time_diff_ns(start_time, current_time);
    // print the time
    printf("Time taken in nanoseconds - %ld\n", elapsed_ns);
    return 0;
}

// subroutine that creates the initial empty grid
void initialize_grid() {
    // Fill the grid with '_'
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            arr[r][c] = '_';
        }
    }
}
// subroutine that adds the queens to the initial grid
void create_grid() {
    // call the initialize_ grid subroutine to get base grid
    initialize_grid();
    // initialize the placed queens count
    int placed_queens = 0;
    // while the number of queens on the board is less than the user size input
    while (placed_queens < size) {
        // get a random column number  that is less than the max size of the grid
        int random_c_num = rand() % size;
        // same for row
        int random_r_num = rand() % size;
        // if the random row and column coordinate is empty then place a Q
        if (arr[random_r_num][random_c_num] == '_') {
            // Place only in an empty spot
            arr[random_r_num][random_c_num] = 'Q';
            // add one to the queen count
            placed_queens++;
        }
    }
}
// subroutine that prints the most recent grid
void print_grid() {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            printf("%c ", arr[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}
// subroutine that checks for the conditions of the queens and sees if the board will be a correct answer
bool check_queens(){
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (arr[r][c] == 'Q') {
                //Check rows
                for (int i = 0; i < size; i++) {
                    if (arr[i][c] == 'Q' && r != i) { return false; }
                }
                //Check columns
                for (int k = 0; k < size; k++) {
                    if (arr[r][k] == 'Q' && c != k) { return false; }
                }
                for (int j = 1; j < size; j++) {
                    // Check bottom-left diagonal
                    if (r + j < size && c - j >= 0 && arr[r + j][c - j] == 'Q') { return false; }
                    // Check bottom-right diagonal
                    if (r + j < size && c + j < size && arr[r + j][c + j] == 'Q') { return false; }
                    // Check top-left diagonal
                    if (r - j >= 0 && c - j >= 0 && arr[r - j][c - j] == 'Q') { return false; }
                    // Check top-right diagonal
                    if (r - j >= 0 && c + j < size && arr[r - j][c + j] == 'Q') { return false; }
                }
            }
        }
    }
    // If all the conditions are met, then return true
    return true;
}
