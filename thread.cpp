#include <pthread.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#define NUM_THREADS     5

struct arg_struct {

};

void *PrintHello(void* arguments)
{
   long tid = rand();
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;

   struct arg_struct args;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, &PrintHello, (void*) &args);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}