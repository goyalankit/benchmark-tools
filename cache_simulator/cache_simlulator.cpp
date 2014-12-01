/*
for address in full_address_trace:
    if address in active_set:
    swap_to_the_top_of_active_set(address);
    else if address in passive_set:   // conflict miss
        set_associative_conflict++;
        passive_set.remove(address);
        add_to_the_top_of_active_set(address);
    else                        // cold miss
        add_to_the_top_of_active_set(address);

add_to_the_top_of_active_set takes care of the fact that set size size is less than or equal to k.


Created by: Ankit Goyal
*/

#include "lru_cache.h"
#include <unordered_map>
#include <set>
#include <fstream>
#include <iostream>
#define NUM_SETS 64
#define ASSOCIATIVITY 8
#define ADDR_TO_CACHE_LINE(x)   (x >> 6)

// creates a bit mask with bits between a and b from the right as 1.
// example: b = 4, a = 2 => 0000001100
#define BIT_MASK(a, b) (size_t)(((size_t) -1 >> ((sizeof(size_t)*8) \
                - (b))) & ~((1U << (a)) - 1))

unsigned address_to_set(size_t address) {
    return ((address & BIT_MASK(6, 12)) >> 6);
}

typedef cache::lru_cache<size_t, size_t> lru_cache_t;
typedef std::unordered_map<size_t,  lru_cache_t*> lru_per_set_t;

// Cache control variables.
lru_per_set_t *lru_per_set;
std::set<size_t> *previously_seen_addresses;

#define EXIST_IN_CACHE(x) (*lru_per_set)[address_to_set(x)]->exists(ADDR_TO_CACHE_LINE(x))
#define GET(x) (*lru_per_set)[address_to_set(x)]->get(ADDR_TO_CACHE_LINE(x))
#define PUT(x) ((*lru_per_set)[address_to_set(x)]->put(ADDR_TO_CACHE_LINE(x), ADDR_TO_CACHE_LINE(x)))
#define ACCESSED_BEFORE(x) ((*previously_seen_addresses).find(ADDR_TO_CACHE_LINE(x)) \
        != (*previously_seen_addresses).end())
#define ACCESS(x) ((*previously_seen_addresses).insert(ADDR_TO_CACHE_LINE(x)))

size_t total_conflicts = 0;

int calculate_cache_conflicts(size_t address) {
    if (EXIST_IN_CACHE(address)){
        // Use the value. this puts the address on top of LRU
        GET(address);
    } else if (ACCESSED_BEFORE(address)){ // not present in cache but has been accessed before
        PUT(address); // this puts the address on top. LRU
        total_conflicts++;
    } else { // first access. cold miss
        PUT(address); // this puts the address on top. LRU
        ACCESS(address);
    }
}

int main(int argc, char *argv[]) {
    // initialization code
    lru_per_set = new lru_per_set_t();
    previously_seen_addresses = new std::set<size_t>();
    for (int i = 0; i < NUM_SETS; ++i) {
        (*lru_per_set)[i] = new lru_cache_t(ASSOCIATIVITY);
    }

    // read the trace from file
    std::ifstream infile(argv[1]);
    size_t address;
    while (infile >> address) {
        calculate_cache_conflicts(address);   
    }
    std::cout << "Total Conflicts: " << total_conflicts << std::endl;
    // free the memory
    delete lru_per_set;
}
