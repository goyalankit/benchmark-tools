void indigo__create_map();
#include <stdio.h>
#include <gsl/gsl_rng.h>
#ifdef PAPI
#define PAPI 1
#endif
#if PAPI
#include <papi.h>
#endif
#define MAX_COUNTERS 256
#define SEQUENCE_CNT 4000
/* global generator */
#include "mrt.h" 
gsl_rng *r;
int rozmer = 4000;
int seed;
static char *events[] = {("PERF_COUNT_HW_CACHE_L1D:WRITE"), ("PERF_COUNT_HW_CACHE_L1D:ACCESS"), ("PERF_COUNT_HW_CACHE_L1D:READ"), ("PERF_COUNT_HW_CACHE_L1D:PREFETCH"), ("PERF_COUNT_HW_CACHE_L1D:MISS"), ("HW_PRE_REQ:L1D_MISS")
//"L1-DCACHE-LOADS"
};
#define NUMCOUNTERS 6
/* 
 * generating random numbers separately to
 * prevent the side effects on hardware counters.
 *
 * */

void generate_random_numbers(int *sequence)
{
  const gsl_rng_type *T;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
/* seed */
  gsl_rng_set(r,seed);
/* default generator type is mt19937, which is what we need */
// printf("generator type: %s\n", gsl_rng_name (r));
  int i;
  for (i = 0; i < 4000; i++) {
// limit the random numbers to [0, 3999]
    sequence[i] = (gsl_rng_get(r) % 4000);
// printf("%u\t", sequence[i]);
  }
}

void compute(int *sequence)
{
#if PAPI
/* Initialize the library */
/* PAPI create event */
#endif
  float matice1[4000UL];
  float matice2[4000UL];
  int j;
  int idx;
  for (j = 0; j < 4000; j++) {
    indigo__record_c(1,94,((void *)(&sequence[j])),0,sizeof(int ));
    idx = sequence[j];
    indigo__record_c(2,95,((void *)(&matice2[idx])),1,sizeof(float ));
    matice2[idx] = (13 % (idx + 1));
    indigo__record_c(2,96,((void *)(&matice1[idx])),2,sizeof(float ));
    indigo__record_c(1,96,((void *)(&matice2[idx])),1,sizeof(float ));
    matice1[idx] = (matice2[idx] * 231);
//  printf("%d\t", idx);
  }
//printf("\n");
#if PAPI
//    fprintf(stderr, "%lld\n ", PAPI_Counters[k]);
#endif
}

void main(int argc,char *argv[])
{
  int sequence[4000UL];
  indigo__init_(1,0);
  indigo__create_map();
  if (argc < 2) {
    printf("Usage %s <seed>\n",argv[0]);
    exit(1);
  }
  else {
    seed = atoi(argv[1]);
  }
#if PAPI
// printf("PAPI enabled.\n");
#else
// printf("PAPI disabled.\n");
#endif
  generate_random_numbers(sequence);
  compute(sequence);
}

void indigo__create_map()
{
  indigo__write_idx_c("sequence",8);
  indigo__write_idx_c("matice2",7);
  indigo__write_idx_c("matice1",7);
}
