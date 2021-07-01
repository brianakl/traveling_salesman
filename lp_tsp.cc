
#include <iostream>
#include "ortools/linear_solver/linear_solver.h"

//objective funciton = cost of path

namespace operations_research{

    void lp_tsp(){


        //test graph
        int dist_m [4][4] = {
            {0, 1, 1, 4},
            {1, 0, 2, 6},
            {1, 2, 0, 2},
            {4, 6, 2, 0}
        };

        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        const double infinity = solver->infinity();

        MPVariable* const x = solver->MakeNumVariable(0.0,infinity,"x");


        // Σ ∀

        //objective function
        //minimise Σ(n,i=1)[Σ(i-1,j=1) [c_ij * x_ij] ]
        
        MPObjective* const objective = solver->MutableObjective();
        objective->SetMinization();
        

        for (int i = 1; i < n; i++){
            for (int j = 1; j < i-1; j++){
                objective->SetCoefficent(edge_weight_map[int_to_edge(i,j)],);
            }
        }
        //constraints
        // ∀(i) 1 <= i <= n     
        // ∀(ij) 1 <= j < i <= n
        // ∀(ij) 1 <= 
        // Σ(j<i)[x_ij] + Σ(j>i)[x_ji] = 2
        // x_ij <= 1
        // x_ij >= 0

        MPVariable* const x = solver->MakeNumVar(0.0,infinity,"x");


        //loop through every possible set of 


        //make constraints

        MPConstraint* const c0 = solver->MakeRowConstraint();

    }

}


int main(){

    lp_tsp();


    return 0;
}