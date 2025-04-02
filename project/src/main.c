#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * @brief Program to see how mmap works.
 *
 * This program takes parameters in main which give information about
 * how many blocks of memory to create, the size of the blocks and
 * the base address where the first one will be situated.
 *
 * @param argc The number of command-line arguments (Automatically given by
 * command-line or other program).
 * @param argArray An array of command-line arguments, where:
 *        - "arg[0]" is the program name.
 *        - "arg[1]" is the amount of blocks of memory to create.
 *        - "arg[2]" is the size of the block of memory.
 *        - "arg[3]" is the base address for the first block of memory.
 *
 * @return int Returns 0 on successful execution, or 1 if the number of
 * arguments is incorrect.
 *
 * @note The program expects exactly three command-line arguments. If the number
 * of arguments is incorrect, it prints a usage message and exits with a
 * non-zero status.
 */

int main(int argc, char* arg[]) {
  // Make sure all parameters were passed
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <n_blocks> <size_of_blocks> <base_addr>\n",
            arg[0]);
    return 1;
  }

  // Get the values from the parameters
  int n_blocks;
  int size_of_blocks;
  int base_addr;

  n_blocks = atoi(arg[1]);
  size_of_blocks = atoi(arg[2]);
  base_addr = atoi(arg[3]);

  // Calculate where the initial block address might be located
  int initial_base_address;
  int temp = base_addr / 4096;
  initial_base_address = 4096 * temp;

  printf("Initially, base address is %d in decimal\n", initial_base_address);

  int possible = initial_base_address;
  while (possible < base_addr) {
    possible += 4096;
  }

  // Check for the closest multiple of 4096 in regards to address
  if (possible - base_addr < base_addr - initial_base_address) {
    // If the possible address is closer to the block address, use that instead
    initial_base_address = possible;
  }
  printf("After checking, the base address is %d in decimal\n",
         initial_base_address);

  // Use a loop that creates mmap per amount of blocks
  int count = 0;
  printf("PID: %d\n", getpid());
  printf("Amount of blocks to be created: %d\n", n_blocks);
  while (n_blocks > 0) {
    int address = initial_base_address + (size_of_blocks * count);
    int* block = mmap(&address, size_of_blocks, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("block %d address: %lx\n", count, block);
    n_blocks--;
    count++;
  }

  // Use this sleep to check the memory on the terminal with pmap
  sleep(100);

  return 0;
}