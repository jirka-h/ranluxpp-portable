#include "ranluxpp.h"
#include <stdio.h>
#include <inttypes.h>

/*************************************************************************
 * The implementation of the Linear Congruential Random Number           *
 * Generator with a large modulus defined by the recurrence:             *
 * x_{i+1} = x_{i} * A mod m                                             *
 * where the recurrence parameters are based on the RANLUX generator:    *
 * the base b = 2^24                                                     *
 * the modulus m = b^24 - b^10 + 1 = 2^576 - 2^240 + 1                   *
 * the multiplier A is a power of a -- A = a^p mod m,                    *
 * where a = m - (m-1)/b                                                 *
 *         = b^24  - b^23  - b^10  + b^9   + 1                           *
 *         = 2^576 - 2^552 - 2^240 + 2^216 + 1                           *
 * is the multiplicative inverse of the base b = 2^24                    *
 * i.e. a * b mod m = 1                                                  *
 *************************************************************************/

// a = m - (m-1)/b = 2^576 - 2^552 - 2^240 + 2^216 + 1
static const uint64_t
initial_a[9] = {0x0000000000000001UL, 0x0000000000000000UL, 0x0000000000000000UL,
	    0xffff000001000000UL, 0xffffffffffffffffUL, 0xffffffffffffffffUL,
	    0xffffffffffffffffUL, 0xffffffffffffffffUL, 0xfffffeffffffffffUL};

//static const uint64_t
//initial_a[9] = {0x0000000000000000UL,0x0000000000000000UL, 0x0000000000000000UL,
//  0x0000000000000000UL, 0x0000000000000000UL, 0x0000000000000000UL,
//  0x0000000000000000UL, 0x0000000000000000UL, 0x8000000000000000UL};

// modulo value m static const uint64_t
//initial_a[9] = {0x0000000000000001UL, 0x0000000000000000UL, 0x0000000000000000UL,
//  0xffff000000000000UL, 0xffffffffffffffffUL, 0xffffffffffffffffUL,
//  0xffffffffffffffffUL,0xffffffffffffffffUL, 0xffffffffffffffffUL};

void ranluxpp_init(ranluxpp_t* r, uint64_t seed, uint64_t p) {
  r->x[0] = 1;
  for(int i=1;i<9;i++) r->x[i] = 0;
  for(int i=0;i<9;i++) r->A[i] = initial_a[i];
  powermod(r->A, r->A, p); // A = a^p mod m. Recommended p = 2048 (skipping!)

  // seed the generator by jumping to the state x_seed = x_0 * A^(2^96 * seed) mod m
  // the scheme guarantees non-colliding sequences
  uint64_t a[9];
  powermod(r->A, a, 1UL<<48); powermod(a, a, 1UL<<48); // skip 2^96 states
  powermod(a, a, seed);                        // skip 2^96*seed states
  mulmod(a, r->x);
  //ranluxpp_fprintf(r, stderr);
}

// LCG -- modular multiplication
void ranluxpp_nextstate(ranluxpp_t* r) {
  //ranluxpp_fprintf(r, stderr);
  mulmod(r->A, r->x);
}

// jump ahead by n 24-bit RANLUX numbers
void ranluxpp_jump(ranluxpp_t* r, uint64_t n){
  uint64_t a[9];
  powermod(initial_a, a, n);
  mulmod(a, r->x);
}

void ranluxpp_fprintf(ranluxpp_t* r, FILE* stream) {
  for (int i=0; i<9; ++i) {
    fprintf(stream, "x[%d]\t%016" PRIx64 "\n", i, r->x[i]);
  }
  for (int i=0; i<9; ++i) {
    fprintf(stream, "A[%d]\t%016" PRIx64 "\n", i, r->A[i]);
  }
}
