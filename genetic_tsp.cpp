#include "TSP.h"
#include <queue>


/*
Genetic TSP
create a population of solutions
    -stating population of random solutions

create a selection function
    -each solution is compared with its neighbors and the better one moves on
    -elitism
        -top 5%? automatically make it through to the next round

*keep track of the very best path across all time

create a breeding function
    -randomly choose a subset of the path from parent 1 and
        fill it with parent 2s path without duplicating cities
    - i.e. Parent 1: 1[2 3 4 5]6 7 8
           Parent 2: 5 2 4 8 7 6 1 3
        offspring 1: 1 5 2 4 3 6 7 8
        
add mutations
    -swapping two random cities in a path 
    -mutation rate = 0.05 ?

*/
void TSP::genetic_starter(int pop){
    //create random starting population
    vector<vector<int> > population;

    vector<int> set;        //set of available verticies for each population
    vector<double> dist;    //distances of each individual in the population


    for(int i = 0; i < pop; i++){
        population.push_back(vector<int>());
        set.clear();
        for (int k = 0; k < n; k++)
            set.push_back(k);
        //creating an individual in the pop
        for(int j = 0; j < n; j++){
            int r = rand() % (n - j);
            population[i].push_back(set[r]);
            set[r] = set.back();
            set.pop_back();
        }
        dist.push_back(distance(population[i]));
    }
    genetic_tsp(population,dist,pop);
    
    print_path(*best_genetic_path);

}

void TSP::genetic_tsp(vector<vector<int> >& population, vector<double>& dist, int pop, int gen){
    //sets limit to number of generations
    if (gen > GEN_LIMIT) return;
    //create a priority queue of the population for elitism
    auto comp = [&](int u, int v){return dist[u] > dist[v];};
    priority_queue <int, vector<int>, decltype(comp)> pq(comp);

    double best_dist_current = INFINITY;

    for (int i = 0; i < pop; i++)
        pq.push(i);
    
    vector<vector<int> > parents;
    vector<vector<int> > offspring;
    //top 5% will go through 
    for (int i = 0; i < (0.05 * pop); i++){
        parents.push_back(population[pq.top()]);
        offspring.push_back(population[pq.top()]);
        vector<int> mutant = population[pq.top()];
        mutate(mutant);
        offspring.push_back(mutant);
        //best distance
        if (dist[pq.top()] < best_genetic_distance){
            best_genetic_path = new vector<int>();
            *best_genetic_path = population[pq.top()];
            best_genetic_distance = distance(population[pq.top()]);
        }
        if (dist[pq.top()] < best_dist_current){
            best_dist_current = dist[pq.top()];
        }

        pq.pop();
    }
    if (gen % 1000 == 0)
        cout << "Best distance of gen " << gen <<  " = " << best_dist_current << endl;
    
    //selection method 1
    vector<int> selected = selection(population,dist,pop);
    while(!selected.empty()){
        parents.push_back(population[selected.back()]);
        selected.pop_back();
    }
    /*
    //selection method 2


    while(pq.size() > (pop * 0.45)){
        if (population[pq.top()].size() == 0){
            pq.pop();
            continue;
        }
        parents.push_back(population[pq.top()]);
        pq.pop();
    }
*/

    //breeding
    for (int i = 1; i < parents.size(); i++){

        offspring.push_back(breed(parents[i],parents[i-1]));
        offspring.push_back(breed(parents[i-1],parents[i]));

        //5% chance of mutation
        int r = rand() % 20;

        if (r == 0) mutate(offspring[i-1]);

    
    }

    vector<double> off_dist;
    int t = 0;

    offspring.erase(offspring.begin()+pop, offspring.end());
    
    for (auto it : offspring){
        off_dist.push_back(distance(it));
    }
    
    genetic_tsp(offspring,off_dist,offspring.size(),++gen);

}



vector<int> TSP::selection(vector<vector<int> >& population, vector<double>& dist, int pop){
    vector<int> ret;
    for(int i =1; i < pop; i += 2){
        if (dist[i] < dist[i-1]){
            ret.push_back(i);
        } else{
            ret.push_back(i-1);
        }
    }
    return ret;
}



vector<int> TSP::breed(vector<int>& p1, vector<int>& p2){
    vector<int> ret;
    int r0 = rand() % n;
    int r1 = rand() % n;
    if (r0 > r1){
        int t = r0;
        r0 = r1;
        r1 = t;
    }
    int p1_map = 0;
    //mark all the cities of p1 from [0,r0) U (r1,n]
    for (int i = 0; i < n; i++){
        if (i < r0 || i > r1){
            p1_map |= (1 << p1[i]);
            ret.push_back(p1[i]);
            continue;
       }
       ret.push_back(-1);
    }

    for (int i = 0; i < n; i++){
        //if p2[i] has already been visited then we skip it
        if (p1_map & (1 << p2[i]))
            continue;
        ret[r0++] = p2[i];
        if (r0 > r1) break;   
    }
    return ret;
}

void TSP::mutate(vector<int>& u){
    srand(time(0));
    int r0 = rand() % n, r1 = rand() % n,t;
    t = u[r0];
    u[r0] = u[r1];
    u[r1] = t;

}