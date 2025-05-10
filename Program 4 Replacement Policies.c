// Owen O'Neil
// Program 4 Replacement Policies
// 4/4/2025
// This code simulates and demonstrates OTP, FIFO, LRU, and CLOCK instruction scheduling

// Get necessary imports
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Create the stream struct with all variables(page requests, each frame, and clock star tracker)
struct stream {
    int page_request;
    int *currentFramesopt;
    int *currentFrameslru;
    int *currentFramesfifo;
    int *currentFramesclock;
    int *clockStars;
};
// Create the page request list
struct stream page_request_list[2000];

// count variable
int count;
// Page request generator subroutine
int GeneratePageRequest(int frames_allowed) {
    // get a random number and add it to its index in the page_request_list and also create a page for each of the schedules
    for (int i = 0; i < 2000; i++) {
        int rand_num = rand() % 513;
        page_request_list[i].page_request = rand_num;
        // Allocate memory for each frame array
        page_request_list[i].currentFramesopt = (int *) malloc(frames_allowed * sizeof(int));
        page_request_list[i].currentFrameslru = (int *) malloc(frames_allowed * sizeof(int));
        page_request_list[i].currentFramesfifo = (int *) malloc(frames_allowed * sizeof(int));
        page_request_list[i].currentFramesclock = (int *) malloc(frames_allowed * sizeof(int));
        page_request_list[i].clockStars = (int *) malloc(frames_allowed * sizeof(int));  // For clock star flags

        // Initialize all frame slots with -1 (empty)
        for (int j = 0; j < frames_allowed; j++) {
            page_request_list[i].currentFramesopt[j] = -1;
            page_request_list[i].currentFrameslru[j] = -1;
            page_request_list[i].currentFramesfifo[j] = -1;
            page_request_list[i].currentFramesclock[j] = -1;
            page_request_list[i].clockStars[j] = 0;  // Initialize stars to 0 (not starred)
        }
    }
    count = 2000;
    return 0;
}
// Create the OPT scheduling subroutine passing through the frames allowed
float OPT(int frames_allowed) {
    int fault_opt = 0;
    // set the current frame to the incoming page request
    for (int i = 0; i < count; i++) {
        int current_page = page_request_list[i].page_request;
        int found = 0;
        // Copy previous frame state
        if (i > 0) {
            for (int f = 0; f < frames_allowed; f++) {
                page_request_list[i].currentFramesopt[f] = page_request_list[i - 1].currentFramesopt[f];
            }
        }
        // Check if current page is already in frames
        for (int j = 0; j < frames_allowed; j++) {
            if (page_request_list[i].currentFramesopt[j] == current_page) {
                found = 1;
                break;
            }
        }
        // If found then no page fault and continue
        if (found) continue;
        // Check for an empty frame first
        int placed = 0;
        // If the frame is empty then place the current page and add a page fault
        for (int j = 0; j < frames_allowed; j++) {
            if (page_request_list[i].currentFramesopt[j] == -1) {
                fault_opt ++;
                page_request_list[i].currentFramesopt[j] = current_page;
                placed = 1;
                break;
            }
        }
        // If it's placed into the empty frame then continue
        if (placed) continue;
        // Page fault
        fault_opt++;
        // Otherwise, find the optimal page to replace
        int farthest_index = -1;
        int frame_to_replace = -1;
        // Check through every frame and see what is the farthest use
        for (int j = 0; j < frames_allowed; j++) {
            int next_use = -1;
            for (int k = i + 1; k < count; k++) {
                if (page_request_list[k].page_request == page_request_list[i].currentFramesopt[j]) {
                    next_use = k;
                    break;
                }
            }
            // Not used again
            if (next_use == -1) {
                frame_to_replace = j;
                break;
            }
            // Set the next use to hte the furthest index if its index is higher
            if (next_use > farthest_index) {
                farthest_index = next_use;
                frame_to_replace = j;
            }
        }
        // Replace the selected frame with the current page
        if (frame_to_replace != -1) {
            page_request_list[i].currentFramesopt[frame_to_replace] = current_page;
        }
    }
    // Calculate the fault rate
    float rate = (float) fault_opt / count;
    return rate;
}
// Create the FIFO scheduling subroutine passing through the frames allowed
float FIFO(int frames_allowed) {
    int fault_fifo = 0;
    // Points to the next frame to replace
    int fifo_pointer = 0;
    // Set current page request
    for (int i = 0; i < count; i++) {
        int current_page = page_request_list[i].page_request;
        int found = 0;
        // Copy previous frame state
        if (i > 0) {
            for (int f = 0; f < frames_allowed; f++) {
                page_request_list[i].currentFramesfifo[f] = page_request_list[i - 1].currentFramesfifo[f];
            }
        }
        // Check if page already in frame
        for (int j = 0; j < frames_allowed; j++) {
            if (page_request_list[i].currentFramesfifo[j] == current_page) {
                found = 1;
                break;
            }
        }
        // If found continue to next index
        if (found) continue;
        // Page fault occurred
        fault_fifo++;
        // Replace the frame at fifo_pointer
        page_request_list[i].currentFramesfifo[fifo_pointer] = current_page;
        // Move pointer to next frame (wrap around if needed)
        fifo_pointer = (fifo_pointer + 1) % frames_allowed;
    }
    // Calculate the fault rate
    float rate = (float) fault_fifo / count;
    return rate;
}
// Create the LRU scheduling subroutine passing through the frames allowed
float LRU(int frames_allowed) {
    int fault_lru = 0;
    // Set current page to the current page request
    for (int i = 0; i < count; i++) {
        int current_page = page_request_list[i].page_request;
        int found = 0;
        // Copy previous frame state
        if (i > 0) {
            for (int f = 0; f < frames_allowed; f++) {
                page_request_list[i].currentFrameslru[f] = page_request_list[i - 1].currentFrameslru[f];
            }
        }
        // Check if current page is already in frames
        for (int j = 0; j < frames_allowed; j++) {
            if (page_request_list[i].currentFrameslru[j] == current_page) {
                found = 1;
                break;
            }
        }
        // If found then continue to next index
        if (found) continue;
        // Check for an empty frame first
        int placed = 0;
        for (int j = 0; j < frames_allowed; j++) {
            // If the frame is empty then add a page fault and bring in the current page
            if (page_request_list[i].currentFrameslru[j] == -1) {
                fault_lru++;
                page_request_list[i].currentFrameslru[j] = current_page;
                placed = 1;
                break;
            }
        }
        // Placed into empty frame, done
        if (placed) continue;
        fault_lru++;
        // Find the least recently used page
        int lru_index = -1;
        // Max possible distance back
        int oldest_use = i;
        for (int j = 0; j < frames_allowed; j++) {
            int last_used = -1;
            // If the page request is the same and the current frame then set the last used
            for (int k = i - 1; k >= 0; k--) {
                if (page_request_list[k].page_request == page_request_list[i].currentFrameslru[j]) {
                    last_used = k;
                    break;
                }
            }
            // Set the oldest use as the last used as long as the last used index is less than oldest use
            if (last_used < oldest_use) {
                oldest_use = last_used;
                lru_index = j;
            }
        }
        // Replace the LRU page with current page
        if (lru_index != -1) {
            page_request_list[i].currentFrameslru[lru_index] = current_page;
        }
    }
    // Calculate the fault rate
    float rate = (float) fault_lru / count;
    return rate;
}
// Create the CLOCK scheduling subroutine passing through the frames allowed
float CLOCK(int frames_allowed) {
    int fault_clock = 0;
    // initialize the clock pointer
    int clock_pointer = 0;
    // Set the current page as the current page request
    for (int i = 0; i < count; i++) {
        int current_page = page_request_list[i].page_request;
        int found = 0;
        // Copy previous frame state
        if (i > 0) {
            // Also copy the frames with stars
            for (int f = 0; f < frames_allowed; f++) {
                page_request_list[i].currentFramesclock[f] = page_request_list[i - 1].currentFramesclock[f];
                page_request_list[i].clockStars[f] = page_request_list[i - 1].clockStars[f];
            }
        }
        // Check if the page is already in memory
        for (int j = 0; j < frames_allowed; j++) {
            if (page_request_list[i].currentFramesclock[j] == current_page) {
                found = 1;
                // Add a star to the frame index with the currant page
                if (page_request_list[i].clockStars[j] == 0) {
                    page_request_list[i].clockStars[j] = 1;
                }
                break;
            }
        }
        // If found continue to the next index
        if (found) continue;
        // page fault
        fault_clock++;
        // Look for a frame to replace
        while (1) {
            int frame_val = page_request_list[i].currentFramesclock[clock_pointer];
            // If there is an empty slot use it and put a star(1) to the same index
            if (frame_val == -1) {
                page_request_list[i].currentFramesclock[clock_pointer] = current_page;
                page_request_list[i].clockStars[clock_pointer] = 1;
                // go to the next frame
                clock_pointer = (clock_pointer + 1) % frames_allowed;
                break;
            }
            else if (page_request_list[i].clockStars[clock_pointer] == 1) {
                // If starred, remove star and move on
                page_request_list[i].clockStars[clock_pointer] = 0;
                clock_pointer = (clock_pointer + 1) % frames_allowed;
            }
            else {
                // Replace non-starred page and add star to that index
                page_request_list[i].currentFramesclock[clock_pointer] = current_page;
                page_request_list[i].clockStars[clock_pointer] = 1;
                // Move the pointer to the next frame
                clock_pointer = (clock_pointer + 1) % frames_allowed;
                break;
            }
        }
    }
    // Calculate the float rate
    float rate = (float) fault_clock / count;
    return rate;
}
// Create the print frame subroutine so output is printed and formatted correctly
void printFrameSnapshot(int index, int frames_allowed) {
    printf("Page Stream\n");

    int start, end;
    // Print 5 indexes before and after the screenshot index
    if (index < 6) {
        start = 0;
        end = index + 5;
    }
    // If there are not 6 indexes print every index before and 5 after the screenshot index
    else {
        start = index - 5;
        end = index + 5;
    }

    // Print all page requests in one line, separated by commas
    for (int i = start; i <= end; i++) {
        printf("%d", page_request_list[i].page_request);
        if (i < end) printf(", ");
    }
    printf("\n");
    printf("OPT\tFIFO\tLRU\tCLOCK\n");\
    // Print the OTP, FIFO, LRU and CLOCK pages at the selected index
    for (int f = 0; f < frames_allowed; f++) {
        int opt_val = page_request_list[index].currentFramesopt[f];
        int fifo_val = page_request_list[index].currentFramesfifo[f];
        int lru_val = page_request_list[index].currentFrameslru[f];
        int clock_val = page_request_list[index].currentFramesclock[f];
        int clock_star = page_request_list[index].clockStars[f];

        // Print OPT
        if (opt_val == -1) printf("-\t");
        else printf("%d\t", opt_val);

        // Print FIFO
        if (fifo_val == -1) printf("-\t");
        else printf("%d\t", fifo_val);

        // Print LRU
        if (lru_val == -1) printf("-\t");
        else printf("%d\t", lru_val);

        // Print CLOCK with star if starred
        if (clock_val == -1) {
            printf("-\n");
        } else if (clock_star) {
            // Print star with the value
            printf("%d*\n", clock_val);
        } else {
            printf("%d\n", clock_val);
        }
    }
}
// Create the Main program subroutine
int main() {
    // initialize the random number generator
    srand(time(0));
    int frames_allowed = 0;
    int frame_snapshot = 0;
    // Get user input
    printf("Frames allowed must be between 2 and 128\n");
    printf("Enter the frames allowed: ");
    scanf("%d", &frames_allowed);
    // Generate the page request
    GeneratePageRequest(frames_allowed);
    float opt_output = OPT(frames_allowed);
    float fifo_output = FIFO(frames_allowed);
    float lru_output = LRU(frames_allowed);
    float clock_output = CLOCK(frames_allowed);
    // Print all the fault rates
    printf("Frame size - %d\n", frames_allowed);
    printf("Algos PF Rate\n");
    printf("OPT   %.4f\n", opt_output);
    printf("FIFO  %.4f\n", fifo_output);
    printf("LRU   %.4f\n", lru_output);
    printf("CLOCK %.4f\n", clock_output);
    while(1){
            //get the frame snapshot through user input
            printf("Which frame snapshot to be displayed: ");
            scanf("%d", &frame_snapshot);
            // If the input is -1, end the program
            if (frame_snapshot == -1) {
                break;
            }
            // Print the output of the frame snapshot subroutine
            else {
                printFrameSnapshot(frame_snapshot, frames_allowed);
            }
    }
    return 0;
}
