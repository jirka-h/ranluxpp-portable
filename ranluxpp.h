#ifndef RANLUXPP_H
#define RANLUXPP_H

#include "mulmod.h"
#include <stdio.h>
#include <inttypes.h>

typedef struct {
  uint64_t x[9];      //State vector
  uint64_t A[9];      //Multiplier
} ranluxpp_t;

// seed the generator by jumping to the state x_seed = x_0 * A^(2^96 * seed) mod m
// the scheme guarantees non-colliding sequences
// Set multiplier A: A = a^p mod m
// Recommended p=2048 (skipping 2048 values)
void ranluxpp_init(ranluxpp_t* r, uint64_t seed, uint64_t p);

void ranluxpp_nextstate(ranluxpp_t* r);

// jump ahead by n 24-bit RANLUX numbers
void ranluxpp_jump(ranluxpp_t* r, uint64_t n);

void ranluxpp_fprintf(ranluxpp_t* r, FILE* stream);

#endif
