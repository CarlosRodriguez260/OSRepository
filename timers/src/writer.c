#include <curses.h> /* ncurses.h incluye stdio.h */
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Writes the elapsed_time data to the elapsed_times.log file in logs folder
void write_elapsed_time(double elapsed_time) {
    static FILE *time_file = NULL;
    
    // Open file in(append mode)
    if (time_file == NULL) {
        time_file = fopen("../logs/elapsed_times.log", "a");
  
        // Write header if file is new
        struct stat st;
        if (fstat(fileno(time_file), &st) == 0 && st.st_size == 0) {
            fprintf(time_file, "Elapsed Time with Timers (ms)\n");
        }
    }
    
    // Write the time measurement
    fprintf(time_file, "%.3f\n", elapsed_time);
    fflush(time_file);  // Ensure data is written immediately
}