#include <iostream>
#include "ortools/linear_solver/linear_solver.h"

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


}


namespace operations_research{
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
}


int main() {

    operations_research::SimpleMipProgram();

    return EXIT_SUCCESS;
}
