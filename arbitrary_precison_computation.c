/*
gcc -Wall -o arbitrary_precison_computation arbitrary_precison_computation.c -lgmp
./arbitrary_precison_computation | sed 's/.\{16\}/& /g'
*/

#include <stdio.h>
#include <gmp.h>

int main(void) {
  mpz_t i;
  mpz_init (i);
  mpz_set_ui(i, 2);
  mpz_pow_ui (i, i, 575); //i=2^575
  mpz_out_str (stdout, 16, i);
  fprintf(stdout,"\n");

  mpz_t base;
  mpz_init (base);
  mpz_set_ui(base, 2);
  mpz_pow_ui (base, base, 24); //b=2^24

  mpz_t m;
  mpz_init (m);
  mpz_pow_ui (m, base, 24); //m=b^24

  mpz_t d;
  mpz_init (d);
  mpz_pow_ui (d, base, 10);
  mpz_sub (m, m, d);        //m=b^24-b^10
  mpz_add_ui (m, m, 1);     //m=b^24-b^10+1
  mpz_out_str (stdout, 16, m);
  fprintf(stdout,"\n");

  //the base b = 2^24                                                     *
  //the modulus m = b^24 - b^10 + 1 = 2^576 - 2^240 + 1

  mpz_powm_ui (i, m, 2, m);  //i = i^2 mod m
  mpz_out_str (stdout, 16, i);
  fprintf(stdout,"\n");
}

