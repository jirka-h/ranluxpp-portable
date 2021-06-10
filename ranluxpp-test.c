/*
gcc -g -O3 -Wall -Wextra -Wpedantic -fsanitize=undefined -I./ -o ranluxpp-test ranluxpp-test.c ranluxpp.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>

#include "ranluxpp.h"

double timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  double n,s;
  n = (double) timeA_p->tv_nsec - (double) timeB_p->tv_nsec;
  s = (double) timeA_p->tv_sec - (double) timeB_p->tv_sec;
  return ( s + n*1.0e-9);
}

const int size=9*1024;

int main(void) {

  ranluxpp_t r;
  ranluxpp_init(&r, 1, 2048);
  //ranluxpp_fprintf(&r, stderr);
  uint64_t buf[size];
  size_t numbers;
  size_t total = 0;
  struct timespec t[4];
  double timeElapsed[2];

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
  clock_gettime(CLOCK_MONOTONIC, &t[2]);

  do {
    for (int i=0; i<1024; i++) {
      //Copy state vector to buffer
      memcpy(buf+9*i, r.x, 9*sizeof(uint64_t));
      //Get next state
      ranluxpp_nextstate(&r);
    }
    numbers = fwrite(buf, sizeof(uint64_t), size, stdout);
    total += numbers;
  } while ( numbers == size );

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]);
  clock_gettime(CLOCK_MONOTONIC, &t[3]);
  timeElapsed[0] = timespecDiff(&t[1], &t[0]);
  timeElapsed[1] = timespecDiff(&t[3], &t[2]);

  fprintf(stderr, "Written %Lg 64-bit numbers\n", (long double) total);
  fprintf(stderr, "WALLCLOCK TIME:\t%g\n"
        "CPU TIME:\t%g\n",
        timeElapsed[1], timeElapsed[0]);

  return 0;
}
