#include "TSP.h"

/*
Genetic TSP
create a population of solutions
    -stating population of 1000 or 500 ?

create a selection function
    -a tournament where the solutions with the lowest distance
        advance and are bred in the next generation
        -two random members are chosen and the one with the lowest distance continues
    -elitism
        -top 5%? automatically make it through to the next round

*keep track of the very best path across all time

create a breeding function
    -randomly choose a subset of the path from parent 1 and
        fill it with parent 2s path without duplicating cities
    - i.e. Parent 1: 1[2 3 4 5]6 7 8
           Parent 2: 5 2 4 8[7 6 1]3
        offspring 1: 1 5 2 4 3 6 7 8
        offsrping 2: 5 2 4 8 1 6 7 3
        
add mutations
    -swapping two cities in a path 
    -mutation rate = 0.05 ?

*/