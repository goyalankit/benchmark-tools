#include <stdio.h>

#define PAPI 1

#if PAPI
#include <papi.h>
#endif
//Array init like this
int rozmer = 1;

#define MAX_COUNTERS 256

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
void compute() {
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

    for (int i = 0; i < NUMCOUNTERS; i++) {
        if ( (retval = PAPI_add_named_event(EventSet, events[i])) != PAPI_OK) {
            fprintf(stderr, "add named event: %s", PAPI_strerror(retval));
        }
    }

    retval = PAPI_start(EventSet);
#endif
    
    float matice1[rozmer][rozmer];
    float matice2[rozmer][rozmer];
    float matice3[rozmer][rozmer];
    //Main multiply code
    int j,k,m;
    for(j = 0; j < rozmer; j++)
    {
        for (k = 0; k < rozmer; k++)
        {
            float temp = 0;
            for (m = 0; m < rozmer; m++)
            {
                temp = temp + matice1[j][m] * matice2[m][k];
            }
            matice3[j][k] = temp;
        }
    }

#if PAPI
    retval = PAPI_stop(EventSet, PAPI_Counters);
    if (retval != PAPI_OK) exit(1);

    for (int j = 0; j < NUMCOUNTERS; j++) {
        printf("%20lld %s\n", PAPI_Counters[j], events[j]);
    //    fprintf(stderr, "%lld\n ", PAPI_Counters[j]);
    }
#endif
}

void main(){
    compute();
}
