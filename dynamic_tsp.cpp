#include "TSP.h"


using namespace std;

double TSP::dynamic_tsp(int cities, int pos){

   double ret = 999999;

    //end case if all nodes are visited
   if (cities == ((1 << n)-1))
       return edge_weight_map[int_to_edge(pos,0)];

    //if the path has already been discovered we can just look it up and return
   if (paths[cities][pos] != -1)
        return paths[cities][pos];

    //else we will call on next unvisited node
    for (int current = 0; current < n; current++){
        if ((cities & (1<<current)) == 0){
            path_xd[key(pos,current)].push_back(pos);
            double a = edge_weight_map[int_to_edge(pos,current)] + dynamic_tsp(cities | (1 << current),current);
            
            if (ret > a) {
                ret = a;
                best_pair = make_pair(pos,current);
            }
        }
    }
    return ret;
}

void TSP::print_dtsp() {
    cout << endl << "dtsp path: " << endl;
    for (auto it : path_xd[key(best_pair.first,best_pair.second)]){
        cout << it << " ";
    }
    cout << endl;
}

