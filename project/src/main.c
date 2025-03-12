#include <curses.h> /* ncurses.h incluye stdio.h */
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

// Global Variables
int BALANCE = 100;
int MAX_THREADS = 20;
int type = -1;  // 0 for deposit, 1 for withdrawal
int transaction_amount = -1;
int amount_per_transaction = -1;
void* showMenu();
void* transaction();
pthread_mutex_t mutex;
pthread_attr_t attr;
pthread_t transactionThread[20], menuThread;

/**
 * @brief Program that displays a bank interface
 * and allows the user to do certain operations
 *
 * @details The program utilizes threads for displaying
 * the interface to the user, as well as threads to handle
 * every transaction the user wants to do. In this case, we
 * utilized the ncurses library to display the interface.
 *
 * @return N/A
 */

int main() {
  pthread_attr_init(&attr);
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&menuThread, &attr, showMenu, NULL);

  // Put main program to sleep while threads work
  while (1) {
    sleep(1);
  }
}

void* showMenu() {
  // Thread in charge of showing the interface to the user
  initscr();
  curs_set(0);

  while (1) {
    clear();
    mvprintw(0, 0, "1. Perform deposits");
    mvprintw(1, 0, "2. Perform withdrawals");
    mvprintw(2, 0, "3. Check balance");
    refresh();

    // Key 1 = 49, Key 2 = 50, Key 3 = 51
    int key_stroke;
    scanw("%d", &key_stroke);
    if (key_stroke != -1) {
      if (key_stroke == 1) {
        // For when the user wants to do deposits
        type = 0;

        clear();
        printw("Type the amount of deposits");
        refresh();
        scanw("%d", &transaction_amount);

        clear();
        printw("Type the amount per deposit");
        refresh();
        scanw("%d", &amount_per_transaction);

        for (int i = 0; i < transaction_amount; i++) {
          if (MAX_THREADS <= 0) {
            clear();
            printw("Max transactions reached!");
            refresh();
            sleep(3);
            break;
          }
          pthread_create(&transactionThread[i], &attr, transaction, NULL);
        }
      } else if (key_stroke == 2) {
        // For when the user wants to do withdrawals
        type = 1;

        clear();
        printw("Type the amount of withdrawals");
        refresh();
        scanw("%d", &transaction_amount);

        clear();
        printw("Type the amount per withdrawal");
        refresh();
        scanw("%d", &amount_per_transaction);

        for (int i = 0; i < transaction_amount; i++) {
          if (MAX_THREADS <= 0) {
            clear();
            printw("Max transactions reached!");
            refresh();
            sleep(3);
            break;
          }
          pthread_create(&transactionThread[i], &attr, transaction, NULL);
        }
      } else if (key_stroke == 3) {
        // For when the user wants to see his balance
        clear();
        printw("Your balance is $%d", BALANCE);
        refresh();
        sleep(3);
      }
    }
    // Use of microsleep to not use too much CPU
    usleep(10000);
  }
}

void* transaction() {
  // Thread in charge of doing the transactions. Depending on the
  // inputs of the user, it will either do deposits or withdrawals.
  // The thread is called an amount of times specified by the
  // user.

  int local_type = type;
  int local_amount_per_transaction = amount_per_transaction;
  pthread_mutex_lock(&mutex);

  MAX_THREADS--;
  if (type == 0) {
    // Deposit
    BALANCE = BALANCE + local_amount_per_transaction;
  } else {
    // Withdrawal
    if (BALANCE - local_amount_per_transaction < 0) {
      // If withdrawal is not possible, don't let
      // the balance go negative
      BALANCE = 0;
    } else {
      BALANCE = BALANCE - local_amount_per_transaction;
    }
  }
  MAX_THREADS++;

  pthread_mutex_unlock(&mutex);
}