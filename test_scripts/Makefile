all: random_access_papi random_access_macpo

.PHONY: check-env check-env-papi

GSL_INC     = $(TACC_GSL_INC)
GSL_INC_GSL = $(TACC_GSL_INC)/gsl
GSL_LIB     = $(TACC_GSL_LIB)
PAPI_INC    = $(TACC_PAPI_INC)
PAPI_LIB    = $(TACC_PAPI_LIB)


random_access_macpo: check-env random_access.c
	macpo.sh -O0 --macpo:disable-sampling --macpo:instrument=compute -I$(GSL_INC_GSL) -I$(GSL_INC) -L$(GSL_LIB) -lgsl -lgslcblas -o random_access_macpo random_access.c

random_access_papi: check-env-papi check-env random_access.c
	macpo.sh -I$(GSL_INC_GSL) -I$(GSL_INC) -L$(GSL_LIB) -lgsl -lgslcblas -lpapi -I$(PAPI_INC) -Wl,-rpath,$(PAPI_LIB) -L$(PAPI_LIB) -o random_access_papi -DPAPI random_access.c

clean:
	rm random_access_macpo random_access_papi random_access.o rose_random_access.c

check-env:
ifndef TACC_GSL_INC
	$(error TACC_GSL_INC is not defined. Trying doing module load gcc4_7/gsl )
endif

check-env-papi:
ifndef TACC_PAPI_INC
	$(error TACC_PAPI_INC is not defined. Trying doing module load papi )
endif
