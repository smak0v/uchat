#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#define NTHREADS 10
#define N 1000
#define MEGEXTRA 1000000

 pthread_attr_t attr;

 void *dowork(void *status)
 {
    long double maths = 1;
    for (int i = 0; i < MEGEXTRA; i++) {
       for (int j = N; j > 0; j--) {
         maths += i * j / N;
       }
    }
    char *exit = (char *)status;
    *exit = 0;
    printf("thread finished\n");
    pthread_exit(NULL);
 }


int main() {
   pthread_t threads[NTHREADS];
   char status[NTHREADS];
   for (int i = 0; i < NTHREADS; i++) {
      status[i] = 0;
   }
   int counter = 0;

   for (int i = 0; i < 10; i++) {
      printf("counter = %d\n", counter);
      for (int i = 0; i < NTHREADS; i++)
         printf("%d ", status[i]);
      printf("\n");
      status[counter] = 1;
      pthread_create(&threads[counter], NULL, dowork, &status[counter]);
      sleep(1);

      for (int j = 0; j < NTHREADS; j++)
         if (status[j] == 0) {
            counter = j;
            break;
         }
   }

   for (int i = 0; i < NTHREADS; i++)
      printf("%d", status[i]);
   printf("\n");
   pthread_exit(NULL);
}

//  int main(int argc, char *argv[])
//  {
//     pthread_t threads[NTHREADS];
//     size_t stacksize;
//     int rc;
//     long t;
//     pthread_attr_init(&attr);
//     pthread_attr_getstacksize (&attr, &stacksize);
//     printf("Default stack size = %li\n", stacksize);
//     stacksize = sizeof(double)*N*N+MEGEXTRA;
//     printf("Amount of stack needed per thread = %li\n", stacksize);
//     pthread_attr_setstacksize (&attr, stacksize);
//     printf("Creating threads with stack size = %s bytes\n", attr.__opaque);
//     for(t=0; t<NTHREADS; t++){
//        rc = pthread_create(&threads[t], &attr, dowork, NULL);
//        if (rc){
//           printf("ERROR; return code from pthread_create() is %d\n", rc);
//           exit(-1);
//        }
//     }
//     printf("Created %ld threads.\n", t);
//     pthread_exit(NULL);
//  }
