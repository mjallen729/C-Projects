#include "virus_cure.h"

static int next_id = 1; //starting caller id
static sem_t connected_lock;
static sem_t operators;
static int num_operators = 5;
static int num_lines = 8;
static int connected = 0; //connected callers (REMINDER: global + static = current file only)
static int orders = 0; //variable to track orders recieved (testing purposes)

void* phonecall(void *arg) {
  sem_init(&connected_lock, 0, 1); //binary semaphore to check if connections are available

  sem_wait(&connected_lock); //locks
    int callid = next_id; //assigns caller ID to thread
    next_id++; //increments global id tracker

  sem_post(&connected_lock); //unlocks

  while (1) { //checking if connections are available
    sem_wait(&connected_lock);
      printf("Caller %d trying to connect...\n", callid);

    if (connected < num_lines) {
      connected++;

      sem_post(&connected_lock); //if connection is available, connect and open lock (connected+1)
      break;

    } else {
      printf("Caller %d dropped, waiting for available line...\n", callid);
      sem_post(&connected_lock); //if connection not available, open lock and wait to try again
      sleep(3);
      continue;

    }

  }

  sem_init(&operators, 0, num_operators); //creates lock with number of operators available

  sem_wait(&operators); //thread connects to an operator
    printf("Caller %d connected! Ringing...\n", callid);
    usleep(500000); //simulating ringing

    printf("Caller %d talking to operator.\n", callid);
    sleep(2); //simulating order placement

    printf("Caller %d placed an order sucessfully!\n", callid);
    orders++; //increments orders variable

  sem_post(&operators); //unlocks an operator slot

  sem_wait(&connected_lock); //updating connected variable with binary semaphore
    printf("Caller %d disconnected.\n", callid);
    connected--; //since caller left, connected slot open

  sem_post(&connected_lock);

  return NULL;

}

int main(int argc, char const *argv[]) {
  char const *cmdline = argv[1]; //points to command line argument array starting at 1

  if (cmdline == NULL) { //if user didn't enter a number of calls
    printf("No calls today!\n");
    exit(1);

  }

  int numcalls = atoi(cmdline); //converts the args (number of callls) into an int

  pthread_t totalcalls[numcalls]; //new array of threads

  for (int i = 0; i < numcalls; i++) {
    usleep(350000);//organizational purposes, output doesn't get bombed by millions of attempts at once (realistic too)
    pthread_create(&totalcalls[i] , NULL, phonecall, NULL); //creating threads for pending calls

  }

  for (int i = 0; i < numcalls; i++) {
    pthread_join(totalcalls[i], NULL); //waiting in the main thread until all phone threads complete

  }

  printf("\nRecieved %d orders!\n", orders);

  return 0;

}
