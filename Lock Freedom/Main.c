#include <assert.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static volatile int gSharedCounter = 0;

int gloopCount;

int lock = 0;

void proc1() {
  bool old;
  while(true) {
    old = 1;
    __sync_bool_compare_and_swap (&old,old,lock);
    if (old == 0)
      ++gSharedCounter;
    lock = 0;
    break;
  }
}

void proc2() { 
  bool old2;
  while(true) {
    old2 = 1;
    __sync_bool_compare_and_swap (&old2,old2,lock);
    if (old2 == 0)
      ++gSharedCounter;
    lock = 0;
    break;
  }
}

//
// Tasks, as a level of indirection
//
void *task1(void *arg) {
  int i;
  printf("Starting task 1\n");
  for(i=0;i<gloopCount;i++) proc1();
}

void *task2(void *arg) {
  int i;
  printf("Starting task 2\n");
  for(i=0;i<gloopCount;i++) proc2();
}

int main(int argc, char ** argv)
{
  int loopCount = 0;
  pthread_t my_thread_1;
  pthread_t my_thread_2;
  void * returnCode;
  int result;
  int expected_sum;

/* Check arguments to program*/
  if(argc != 2)  {
    fprintf(stderr, "USAGE: %s <loopcount>\n", argv[0]); 
    exit(1);
}

/* Parse argument */
  loopCount = atoi(argv[1]);
  gloopCount = loopCount;
  expected_sum = 2*gloopCount;

/* Start the threads */
  result = pthread_create(&my_thread_1, NULL, task1, NULL);
  result = pthread_create(&my_thread_2, NULL, task2, NULL);

/* Wait for the threads to end */
  result = pthread_join(my_thread_1,&returnCode);
  result = pthread_join(my_thread_2,&returnCode); 
  printf("Both threads terminated\n");

/* Check result */
  if( gSharedCounter != expected_sum ) {
     printf("[-] Sum %d rather than %d.\n", gSharedCounter, expected_sum);
     return 1;
  } else {
     printf("[+] Ok\n");
     return 0;
  }
}