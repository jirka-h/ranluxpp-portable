/*
gcc -g -O3 -Wall -Wextra -Wpedantic -fsanitize=undefined -I./ -o ranluxpp-test ranluxpp-test.c ranluxpp.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include <argp.h>
#include <signal.h>

#include "ranluxpp.h"

double timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  double n,s;
  n = (double) timeA_p->tv_nsec - (double) timeB_p->tv_nsec;
  s = (double) timeA_p->tv_sec - (double) timeB_p->tv_sec;
  return ( s + n*1.0e-9);
}

const int size=9*1024;

const char *argp_program_version = "ranluxpp-test 1.0";
const char *argp_program_bug_address = "<hladky.jiri@gmail.com>";
static char doc[] = "Test ranluxpp portable implementation";
/* The options we understand. */
static struct argp_option options[] = {
  {"number",   'n', "NUMBER", 0, "How many 64-bit numbers to generate", 0},
  {"fwrite",   'w', 0,        0, "Output values", 0 },
  { 0 }
};
/* Used by main to communicate with parse_opt. */
struct arguments
{
  size_t n;
  int fwrite;
};
/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'n':
      arguments->n = (size_t) strtod(arg, NULL);
      break;
    case 'w':
      arguments->fwrite = 1;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num > 0)
        /* Too many arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc, 0, 0, 0 };
int main(int argc, char **argv) {
  signal(SIGPIPE, SIG_IGN);
  struct arguments arguments;
  /* Default values. */
  arguments.n = 1.0e9;
  arguments.fwrite = 0;

  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  ranluxpp_t r;
  ranluxpp_init(&r, 1, 2048);
  //ranluxpp_fprintf(&r, stderr);
  uint64_t buf[size];
  size_t numbers;
  size_t total = 0;
  struct timespec t[4];
  double timeElapsed[2];
  uint64_t last;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[0]);
  clock_gettime(CLOCK_MONOTONIC, &t[2]);

  if (arguments.fwrite) {
    do {
      for (int i=0; i<1024; i++) {
        //Copy state vector to buffer
        memcpy(buf+9*i, r.x, 9*sizeof(uint64_t));
        //Get next state
        ranluxpp_nextstate(&r);
      }
      numbers = fwrite(buf, sizeof(uint64_t), size, stdout);
      total += numbers;
    } while ( numbers == size && total < arguments.n);
  } else {
    do {
      ranluxpp_nextstate(&r);   //Generates 576-bits number of 9x64-bits numbers
      total += 9;
    } while ( total < arguments.n);
  }
  last = r.x[0];

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t[1]);
  clock_gettime(CLOCK_MONOTONIC, &t[3]);
  timeElapsed[0] = timespecDiff(&t[1], &t[0]);
  timeElapsed[1] = timespecDiff(&t[3], &t[2]);

  if (arguments.fwrite) {
    fprintf(stderr, "Written %Lg 64-bit numbers\n", (long double) total);
  } else {
    fprintf(stderr, "Generated %Lg 64-bit numbers, last value is %" PRIu64 "\n", (long double) total, last);
  }
  fprintf(stderr, "WALLCLOCK TIME:\t%g\n"
        "CPU TIME:\t%g\n",
        timeElapsed[1], timeElapsed[0]);

  return 0;
}
