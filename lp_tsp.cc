#include <iostream>
#include <vector>
#include "ortools/linear_solver/linear_solver.h"

//objective funciton = cost of path
using namespace std;

namespace operations_research{

    void lp_tsp(){


        //test graph
        //shortest path = 0 -> 1 -> 2 -> 3 -> 0
        double dist_m [4][4] = {
            {0, 1, 1, 4},
            {8, 0, 2, 6},
            {9, 7, 0, 2},
            {3, 6, 5, 0}
        };
        int n = 4;
        vector<vector<MPVariable*> >* vec;

        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        const double infinity = solver->infinity();

        //MPVariable* const x = solver->MakeNumVar(0.0,infinity,"x");
        //MakeNumVarArray(double upper bound, lower bound, name, vector)
        // 0 <= x_ij <= 1
        
        for(int i = 0; i < n; i++)
            vec->push_back(vector<MPVariable*>());
        
        MPVariable* const x = solver->MakeNumVarArray(n, 0.0, 1.0, "row", &vec[i]);
            

       //MakeNumVar(double lower bound, double upper bound, var name)
       //MPVariable* const x = solver->MakeNumVar(0.0,1.0, "x");
       
       LOG(INFO) << "Number of variables:\t" << solver->NumVariables();

        // Σ ∀

        //objective function
        //minimize Σ(n,i=1)[Σ(i-1,j=1)[c_ij * x_ij] ]
        
        MPObjective* const objective = solver->MutableObjective();
        objective->SetMinimization();        

        for (int i = 1; i < n; i++){
            for (int j = 1; j < i-1; j++){
                objective->SetCoefficient(vec[i][j], dist_m[i][j]);
            }
        }
        //constraints
        // ∀(i) 1 <= i <= n     
        // ∀(ij) 1 <= j < i <= n
        // ∀(ij) 1 <= 
        // Σ(j<i)[x_ij] + Σ(j>i)[x_ji] = 2


        //loop through every possible set of 


        //make constraints

        MPConstraint* const c0 = solver->MakeRowConstraint();

    }

}


int main(){

    operations_research::lp_tsp();


    return 0;
}