#include <stdio.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

gsl_rng *gBaseRand;       /* global rand number generator */

int main (void) {
    unsigned long randSeed;
    int i, k, n = 10;
    double lambda = 3.0;

    /* specifying to use Mersenne twister MT-19937 as the uniform PRNG */
    gBaseRand = gsl_rng_alloc(gsl_rng_mt19937);

    srand(time(NULL));                    /* initialization for rand() */
    randSeed = rand();                    /* returns a non-negative integer */
    gsl_rng_set (gBaseRand, randSeed);    /* seed the PRNG */

    /* print n random variates chosen from  the poisson distribution with mean 
       parameter lambda */
    for (i = 0; i < n; i++) {
        k = gsl_ran_poisson (gBaseRand, lambda);
        printf (" %u", k);
    }

    printf ("\n");
    gsl_rng_free(gBaseRand);
    return 0;
}
