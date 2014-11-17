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

gsl_rng * r; /* global generator */


int rozmer = SEQUENCE_CNT;
int seed;

static char * events[] = {
    "PERF_COUNT_HW_CACHE_L1D:WRITE", 
    "PERF_COUNT_HW_CACHE_L1D:ACCESS",
    "PERF_COUNT_HW_CACHE_L1D:READ",
    "PERF_COUNT_HW_CACHE_L1D:PREFETCH",
    "PERF_COUNT_HW_CACHE_L1D:MISS",
    "HW_PRE_REQ:L1D_MISS",
    //"L1-DCACHE-LOADS"
};

#define NUMCOUNTERS 6

/* 
 * generating random numbers separately to
 * prevent the side effects on hardware counters.
 *
 * */
void generate_random_numbers(int *sequence) {
  const gsl_rng_type * T;

  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  /* seed */
  gsl_rng_set(r, seed);
  
  /* default generator type is mt19937, which is what we need */
  // printf("generator type: %s\n", gsl_rng_name (r));

  int i;
  for (i = 0; i < SEQUENCE_CNT; i++) {
    sequence[i] = gsl_rng_get(r) % SEQUENCE_CNT; // limit the random numbers to [0, 3999]
    // printf("%u\t", sequence[i]);
  }
}


void compute(int *sequence) {
#if PAPI
    int EventCode, retval;
    int EventSet = PAPI_NULL;
    long long PAPI_Counters[MAX_COUNTERS];

    /* Initialize the library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);

    if  (retval != PAPI_VER_CURRENT) {
        fprintf(stderr, "PAPI library init error!\n");
        exit(1); 
    }

    /* PAPI create event */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr, "create event set: %s", PAPI_strerror(retval));
    }

    int i;
    for (i = 0; i < NUMCOUNTERS; i++) {
        if ( (retval = PAPI_add_named_event(EventSet, events[i])) != PAPI_OK) {
            fprintf(stderr, "add named event: %s", PAPI_strerror(retval));
        }
    }

    retval = PAPI_start(EventSet);
#endif
    
    float matice1[SEQUENCE_CNT];
    float matice2[SEQUENCE_CNT];
    int j, idx;
    for(j = 0; j < SEQUENCE_CNT; j++)
    {
        idx = sequence[j];
        matice2[idx] = 13 % (idx + 1);
        matice1[idx] = matice2[idx] * 231;
      //  printf("%d\t", idx);
    }

    //printf("\n");
#if PAPI
    retval = PAPI_stop(EventSet, PAPI_Counters);

    if (retval != PAPI_OK) exit(1);
    int k;
    for (k = 0; k < NUMCOUNTERS; k++) {
        printf("%20lld %s\n", PAPI_Counters[k], events[k]);
    //    fprintf(stderr, "%lld\n ", PAPI_Counters[k]);
    }
#endif
}

void main(int argc, char* argv[]){
    int sequence[SEQUENCE_CNT];
    if (argc < 2){
        printf("Usage %s <seed>\n", argv[0]);
        exit(1);
    } else {
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
