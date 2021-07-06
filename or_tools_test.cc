#include <iostream>
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/sat/cp_model.h"


using namespace std;

//integer programing solution
namespace operations_research{
    void SimpleMipProgram(){
        //SCIP = Solving Constraint Integer Programs
        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        if (!solver){
            LOG(WARNING) << "SCIP solver unavailable.";
        }

        const double infinity = solver->infinity();
        //x and y are integer non-negative variables
        //MakeNumVar(double lower bound, double upper bound, var name)
        MPVariable* const x = solver->MakeNumVar(0.0,infinity,"x");
        MPVariable* const y = solver->MakeNumVar(0.0,infinity,"y");
        MPVariable* const z = solver->MakeIntVar(0.0,infinity,"z");

        LOG(INFO) << "Number of variables = " << solver->NumVariables();

        //3x + 2y + 5z <= 55
        //MakeRowConstraint(lowerbound, upperbound, constraint name)
        //default bounds are +- infinity
        MPConstraint* const c0 = solver->MakeRowConstraint(-infinity, 55, "c0");
        c0->SetCoefficient(x,3);
        c0->SetCoefficient(y,2);
        c0->SetCoefficient(z,5);

/*
        //x, y, z >= 0
        MPConstraint* const c1 = solver->MakeRowConstraint(-infinity, 0, "c1");
        c1->SetCoefficient(x,1);
        c1->SetCoefficient(y,1);
        c1->SetCoefficient(z,1);
        you set this constraint using 
        MPconstraint* const c# = 
*/
        //2x + y + z <= 26
        MPConstraint* const c2 = solver->MakeRowConstraint(-infinity, 26,"c2");
        c2->SetCoefficient(x,2);
        c2->SetCoefficient(y,1);
        c2->SetCoefficient(z,1);

        //x + y + z <= 30
        MPConstraint* const c3 = solver->MakeRowConstraint(-infinity, 30, "c3");
        c3->SetCoefficient(x,1);
        c3->SetCoefficient(y,1);
        c3->SetCoefficient(z,1);

        //5x + 2y + 4z <= 57
        MPConstraint* const c4 = solver->MakeRowConstraint(-infinity, 57, "c4");
        c4->SetCoefficient(x,5);
        c4->SetCoefficient(y,2);
        c4->SetCoefficient(z,4);

        LOG(INFO) << "Number of constraints = " << solver->NumConstraints();

        //defining the objective
        //maximize 20x + 10 * y + 15z
        MPObjective* const objective = solver->MutableObjective();
        objective->SetCoefficient(x,20);
        objective->SetCoefficient(y,10);
        objective->SetCoefficient(z,15);
        //you can do SetMinimization() as well
        objective->SetMaximization();

        const MPSolver::ResultStatus result_status = solver->Solve();
        //verifies that the problem has an optimal solution
        if (result_status != MPSolver::OPTIMAL){
            LOG(FATAL) << "The problem does not have an optimal solution";
        }

        //display the solution
        LOG(INFO) << "Solution: ";
        LOG(INFO) << "Objective value = " << objective->Value();
        LOG(INFO) << "x = " << x->solution_value();
        LOG(INFO) << "y = " << y->solution_value();
        LOG(INFO) << "z = " << z->solution_value();
        

    }

    void BasicExample(){
        //creates the linear solver wiht glop
        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        MPVariable* const x = solver->MakeNumVar(0.0, 1, "x");
        MPVariable* const y = solver->MakeNumVar(0.0, 2, "y");

        LOG(INFO) << "Number of varibles = " << solver->NumVariables();

        //create a linear constraint, 0 <= x + y <= 2
        MPConstraint* const ct = solver->MakeRowConstraint(0.0, 2.0, "ct");
        ct->SetCoefficient(x,1);
        ct->SetCoefficient(y, 1);

        LOG(INFO) << "Number of Constraints = " << solver->NumConstraints();

        //objective funciton 3*x + y
        MPObjective* const objective = solver->MutableObjective();
        objective->SetCoefficient(x,3);
        objective->SetCoefficient(y,1);
        objective->SetMaximization();

        solver->Solve();

        LOG(INFO) << "Solution: " << endl;
        LOG(INFO) << "Objective value = " << objective->Value();
        LOG(INFO) << "x = " << x->solution_value();
        LOG(INFO) << "y = " << y->solution_value();

    }

    void lp_tsp(){

        //test graph
        //shortest path = 0 -> 1 -> 2 -> 3 -> 0
        int dist_m [4][4] = {
            {int(INFINITY), 1, 1, 4},
            {8, int(INFINITY), 2, 6},
            {9, 7, int(INFINITY), 2},
            {3, 6, 5, int(INFINITY)}
        };
        int n = 4;
        vector<vector<MPVariable*>* >* vec;
        vector<vector<MPConstraint*>* >* constraints;
        vector<MPVariable*> *v;

        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        const double infinity = solver->infinity();

        //MPVariable* const x = solver->MakeNumVar(0.0,infinity,"x");
        //MakeNumVarArray(double upper bound, lower bound, name, vector)
        // 0 <= x_ij <= 1
        
        //solver->MakeNumVarArray(n, 0.0, 1.0, "row0", v);


        for(int i = 0; i < n; i++){
            vec->push_back(new vector<MPVariable*>());
            for (int j = 0; j < n; j++){
                vec->at(i)->at(j) = solver->MakeIntVar(0.0, 1.0, "x_" + to_string(i) + to_string(j) );
            }
        }

        LOG(INFO) << "Number of variables:\t" << solver->NumVariables();
        //MPConstraint* const c0 = solver->MakeRowConstraint();
        //c0->SetCoefficient();

        //setting constraints
        for(int i = 0; i < n; i++){
            constraints->push_back(new vector<MPConstraint*>());
            for (int j = 0; j < n; j++){
                constraints->at(i)->at(j) = solver->MakeRowConstraint();
                constraints->at(i)->at(j)->SetCoefficient(vec->at(i)->at(j), dist_m[i][j]);
            }
        }

        //solver->MakeNumVarArray(n, 0.0, 1.0, "row0", vec->at(0));
        //solver->MakeNumVarArray(n, 0.0, 1.0, "row1", vec->at(1));            
        //solver->MakeNumVarArray(n, 0.0, 1.0, "row2", vec->at(2));
        //solver->MakeNumVarArray(n, 0.0, 1.0, "row3", vec->at(3));

       //MakeNumVar(double lower bound, double upper bound, var name)
       //MPVariable* const x = solver->MakeNumVar(0.0,1.0, "x");
       

        // Σ ∀

        //objective function
        //minimize Σ(n,i=1)[Σ(n,j=1)[c_ij * x_ij] ]
        
        MPObjective* const objective = solver->MutableObjective();
        objective->SetMinimization();        

        for (int i = 1; i < n; i++){
            for (int j = 1; j < i-1; j++){
                objective->SetCoefficient(vec->at(i)->at(j), dist_m[i][j]);
            }
        }
        //constraints
        // ∀(i) 1 <= i <= n     
        // ∀(ij) 1 <= j < i <= n
        // Σ(j<i)[x_ij] + Σ(j>i)[x_ji] = 2


        //loop through every possible set of 


        //make constraints

        //MPConstraint* const c0 = solver->MakeRowConstraint();

    }

    namespace sat{
        void tsp_cp(){
            CpModelBuilder cp_model;
            double dist_m [4][4] = {
                {0, 1, 1, 4},
                {8, 0, 2, 6},
                {9, 7, 0, 2},
                {3, 6, 5, 0}
            };
            int n = 4;

            const Domain domain(0.0,1.0);

            //variables
            vector<vector<IntVar> > v;

            for (int i = 0; i < n; i++){
                v.push_back(vector<IntVar>());
                for (int j = 0; j < n; j++){
                    if (i == j) continue;
                    v[i].push_back(cp_model.NewIntVar(domain).WithName("x_" + to_string(i)+ to_string(j)));
                }
            }

            for (int i = 0; i < n; i++){
                for (int j = 0; j <n; j++)
                    v[i][j].AddConstant(dist_m[i][j]);
            }

            const IntVar x_01 = cp_model.NewIntVar(domain).WithName("x01");




            /*
            const IntVar x_02 = cp_model.NewIntVar(domain).WithName("x02");
            const IntVar x_03 = cp_model.NewIntVar(domain).WithName("x03");
            const IntVar x_10 = cp_model.NewIntVar(domain).WithName("x10");
            const IntVar x_12 = cp_model.NewIntVar(domain).WithName("x12");
            const IntVar x_13 = cp_model.NewIntVar(domain).WithName("x13");
            const IntVar x_20 = cp_model.NewIntVar(domain).WithName("x20");
            const IntVar x_21 = cp_model.NewIntVar(domain).WithName("x21");
            const IntVar x_23 = cp_model.NewIntVar(domain).WithName("x23");
            const IntVar x_30 = cp_model.NewIntVar(domain).WithName("x30");
            const IntVar x_31 = cp_model.NewIntVar(domain).WithName("x31");
            const IntVar x_32 = cp_model.NewIntVar(domain).WithName("x32");
            */
            //constraints
            
            


            
        }
    }
}


int main() {

    operations_research::SimpleMipProgram();

    operations_research::sat::tsp_cp();

    return EXIT_SUCCESS;
}
