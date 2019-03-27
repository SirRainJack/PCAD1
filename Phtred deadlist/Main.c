#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
void *foo(void *vargp) {
  int myid;
  myid = *((int *)vargp);
  printf("Thread %d\n", myid);
  return(0);
}
int main() {
  pthread_t tid[5];
  void *ret;
  for (int i = 0; i < 5; i++) {
     pthread_create(&tid[i], 0, foo, &i);
  }
  for (int i = 0; i < 5; i++) {
     pthread_join(tid[i],&ret);
  }
  return 0;
}