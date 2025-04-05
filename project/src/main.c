/**
 * @file main.c
 * @brief A multi-threaded CPU/thread manager with ncurses interface.
 *
 * This program launches 4 threads that continuously generate random numbers
 * and test for primality. Each thread runs on a configurable CPU core and niceness value.
 * An ncurses menu allows the user to dynamically reassign CPUs and niceness
 * values to threads.
 *
 * An additional thread also indicates which thread found a prime number most recently.
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <curses.h> /* ncurses.h incluye stdio.h */
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NICE_MIN -20
#define NICE_MAX 19

int number_thread = -1;
bool start = false;
void * found_thread();
typedef struct {
    int id;
    int cpu;
    int nice;
    bool running;
    pthread_t thread;
} thread_info_t;

thread_info_t threads[NUM_THREADS];

/**
 * @def is_prime(int n)
 * @brief Checks whether a number is a prime number.
 *
 * @param n The number to check.
 * @return true if the number is prime, false otherwise.
 */
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i*i <= n; ++i)
        if (n % i == 0)
            return false;
    return true;
}

/**
 * @brief Thread function for generating random numbers and checking for primes.
 *
 * Continuously generates random numbers and tests them for primality.
 * Each thread also applies its current CPU affinity and niceness level.
 * If a prime number is found, it updates a global variable to indicate
 * which thread found it.
 *
 * @param arg Pointer to a thread_info_t structure for this thread.
 * @return NULL
 */
void * prime_thread_func(void * arg) {
    thread_info_t* info = (thread_info_t*)arg;

    while (1) {
        // Apply current CPU affinity
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(info->cpu, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

        // Apply current nice value
        setpriority(PRIO_PROCESS, 0, info->nice);

        int num = rand();
        if (is_prime(num)) {
            //printf("Thread %d (CPU %d, Nice %d) found prime: %d\n", info->id, info->cpu, info->nice, num);
            number_thread = info->id;

            fflush(stdout);
            usleep(100000);
        }
    }
    return NULL;
}

/**
 * @brief Function for helper thread to display which thread last found a prime number.
 *
 * This function runs in a separate thread and updates a specific line
 * in the ncurses menu whenever a new prime number is detected by any of the threads.
 *
 * @return NULL
 */

void * found_thread() {

  // If the main program hasn't printed out the menu yet, wait for it
  while(!start){
    usleep(1000);
  }

  while(1){

    // If the user is typing in something, wait for it
    while(!start){
      usleep(1000);
    }

    char thread_id[20];
    sprintf(thread_id, "%d", number_thread);
    mvprintw(NUM_THREADS + 2,0,"Found prime in thread %s", thread_id);
    refresh();
    usleep(100000);
  }
  return NULL;
}

/**
 * @brief Initializes and runs the ncurses menu.
 *
 * Displays a menu to select one of the 4 threads and modify its CPU
 * affinity or niceness value. Also starts an internal helper thread
 * that updates in the menu when a prime is found and in which thread.
 *
 * Uses arrow keys to select threads, and binds:
 * - 'c' to change CPU
 * - 'n' to change niceness
 * - 'q' to quit
 *
 * @return N/A
 */
void run_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // Initialize the indicator thread
    pthread_t foundThread;
    pthread_attr_t attr1;
    pthread_attr_init(&attr1);
    pthread_create(&foundThread, &attr1, found_thread, NULL);

    int selected_thread = 0;
    int choice;
    int max_cpus = sysconf(_SC_NPROCESSORS_ONLN); // Used for getting amount of CPU cores in system

    while (1) {
        clear();
        mvprintw(0, 0, "Thread Manager (Use arrow keys to select, 'c' to change CPU, 'n' to change Niceness, 'q' to quit)");
        
        for (int i = 0; i < NUM_THREADS; ++i) {
            if (i == selected_thread)
                attron(A_REVERSE);
            mvprintw(i + 2, 0, "Thread %d - CPU: %d, Nice: %d", i, threads[i].cpu, threads[i].nice);
            if (i == selected_thread)
                attroff(A_REVERSE);
        }
        refresh();
        start = true;

        choice = getch();

        switch (choice) {
            case KEY_UP:
                selected_thread = (selected_thread - 1 + NUM_THREADS) % NUM_THREADS;
                break;
            case KEY_DOWN:
                selected_thread = (selected_thread + 1) % NUM_THREADS;
                break;
            case 'c': {
                echo();
                start = false;
                mvprintw(NUM_THREADS + 4, 0, "Enter new CPU (0-%d): ", max_cpus - 1);
                int new_cpu;
                scanw("%d", &new_cpu);
                if (new_cpu >= 0 && new_cpu < max_cpus) {
                    threads[selected_thread].cpu = new_cpu;
                } else {
                    mvprintw(NUM_THREADS + 5, 0, "Invalid CPU.");
                }
                noecho();
                break;
            }
            case 'n': {
                echo();
                start = false;
                mvprintw(NUM_THREADS + 4, 0, "Enter new Niceness (%d to %d): ", NICE_MIN, NICE_MAX);
                int new_nice;
                scanw("%d", &new_nice);
                if (new_nice >= NICE_MIN && new_nice <= NICE_MAX) {
                    threads[selected_thread].nice = new_nice;
                } else {
                    mvprintw(NUM_THREADS + 5, 0, "Invalid Niceness.");
                }
                noecho();
                break;
            }
            case 'q':
                endwin();
                exit(0);
        }
      refresh();
    }
}

/**
 * @brief Entry point of the program.
 *
 * Initializes thread information and starts 4 threads to check for primes.
 * Then launches the ncurses menu function to allow user interaction.
 *
 * @return 0
 */
int main() {
  // Seeds random function with time
  srand(time(NULL));

  // Initialize thread info and start threads
  for (int i = 0; i < NUM_THREADS; ++i) {
      threads[i].id = i;
      threads[i].cpu = i % sysconf(_SC_NPROCESSORS_ONLN);
      threads[i].nice = 0;
      pthread_create(&threads[i].thread, NULL, prime_thread_func, &threads[i]);
  }

  // Start ncurses
  run_ui();

  return 0;
}