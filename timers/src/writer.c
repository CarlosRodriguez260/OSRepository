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

/**
 * @brief Writes the elapsed time data to the elapsed_times.log file in the logs folder.
 * 
 * This function opens or creates the `elapsed_times.log` file located in the `logs` directory. 
 * If the file is new, it writes a header line. The elapsed time is written as a float 
 * value in milliseconds with 3 decimal places, followed by a newline.
 * The file is updated each time the function is called.
 *
 * @param elapsed_time The elapsed time in milliseconds (measured by the program).
 * 
 * @note The file is opened in append mode (`a`), so it does not overwrite the previous log data. 
 * It ensures that data is immediately written to disk by using `fflush`.
 */

void write_elapsed_time(double elapsed_time) {
    static FILE *time_file = NULL;
    
    // Open file, or create it if it doesn't exist
    if (time_file == NULL) {
        time_file = fopen("../logs/elapsed_times.log", "a");
  
        // Write header if file is new
        struct stat st;
        if (fstat(fileno(time_file), &st) == 0 && st.st_size == 0) {
            fprintf(time_file, "Elapsed Time with Thread usleep (ms)\n");
        }
    }
    
    // Write the time measurement
    fprintf(time_file, "%.3f\n", elapsed_time);
    fflush(time_file);  // Ensure data is written immediately
}
