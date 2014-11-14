#include <stdio.h>
#include <gsl/gsl_rng.h>

gsl_rng * r;  /* global generator */

int
main (void)
{
  const gsl_rng_type * T;

  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  
  printf("generator type: %s\n", gsl_rng_name (r));
  printf("seed = %u\n", gsl_rng_default_seed);
  for (int i = 0; i < 100; i++) {
      printf("%u\t", gsl_rng_get (r) % 100);
  }
  printf("\n");
  return 0;
}

