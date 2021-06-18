#include "TSP.h"


using namespace std;

double TSP::dynamic_tsp(int cities, int pos){
   double ret = INFINITY;
    //end case if all nodes are visited
   if (cities == ((1 << n) - 1)){
       parent[0] = pos;
       return edge_weight_map[int_to_edge(pos,0)];
   }

    //if the path has already been discovered we can just look it up and return
   if (dp[cities][pos] != -1){
        return dp[cities][pos];
   }
    int next_city = pos;

    //else we will call on next unvisited node
    for (int current = 0; current < n; current++){
        if ((cities & (1 << current)) == 0){
            double a = edge_weight_map[int_to_edge(pos,current)] + dynamic_tsp(cities | (1 << current),current);
            
            if (ret > a) {
                ret = a;
                //vector of size n that stores the best path
                parent[current] = pos;
                next_city = current;

            }
        }
    }
    dp[cities][pos] = ret;
    path[cities][pos] = next_city;
    return ret;
}

void TSP::print_dtsp() {
vector<int> ret;
    cout << endl << "dtsp path: " <<endl;

    for (auto it : parent)
        cout << it << " ";
    bool duplicates[n];
    for (int i =0; i < n; i++) duplicates[i] = false;

    for (auto it : parent){
        if (duplicates[it])continue;
        ret.push_back(it);
        duplicates[it] = true;
    }
    parent = ret;
    

    print_path(parent);
}

int TSP::ith_bit(int x){
    int ret = 0;
    for (int i = 0; i < n; i++){
        if((x & ( 1 << i )) == 1) ret = i;
    }
    return ret;
}




