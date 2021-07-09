#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/sat/cp_model.h"
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"



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

        unique_ptr<MPSolver> solver(MPSolver::CreateSolver("GLOP"));
        const double infinity = solver->infinity();

        //MPVariable* const x = solver->MakeNumVar(0.0,infinity,"x");
        //MakeNumVarArray(double upper bound, lower bound, name, vector)
        // 0 <= x_ij <= 1
        
        //solver->MakeNumVarArray(n, 0.0, 1.0, "row0", v);

        /*
        for(int i = 0; i < n; i++){
            vec->push_back(new vector<MPVariable*>());
            for (int j = 0; j < n; j++){
                vec->at(i)->at(j) = solver->MakeIntVar(0.0, 1.0, "x_" + to_string(i) + to_string(j) );
            }
        }

        */
        MPVariable* const x_0 = solver->MakeIntVar(0.0, 3.0, "x_0");
        MPVariable* const x_1 = solver->MakeIntVar(0.0, 3.0, "x_1");
        MPVariable* const x_2 = solver->MakeIntVar(0.0, 3.0, "x_2");
        MPVariable* const x_3 = solver->MakeIntVar(0.0, 3.0, "x_3");


        

        LOG(INFO) << "Number of variables:\t" << solver->NumVariables();
        MPConstraint* const c0 = solver->MakeRowConstraint();
        
        //c0->SetCoefficient();

        //setting constraints
        /*
        for(int i = 0; i < n; i++){
            constraints->push_back(new vector<MPConstraint*>());
            for (int j = 0; j < n; j++){
                constraints->at(i)->at(j) = solver->MakeRowConstraint();
                constraints->at(i)->at(j)->SetCoefficient(vec->at(i)->at(j), dist_m[i][j]);
            }
        }



        solver->MakeNumVarArray(n, 0.0, 1.0, "row1", vec->at(1));            
        solver->MakeNumVarArray(n, 0.0, 1.0, "row2", vec->at(2));
        solver->MakeNumVarArray(n, 0.0, 1.0, "row3", vec->at(3));

        MakeNumVar(double lower bound, double upper bound, var name)
        MPVariable* const x = solver->MakeNumVar(0.0,1.0, "x");
       

        // Σ ∀

        //objective function
        //minimize Σ(n,i=1)[Σ(n,j=1)[c_ij * x_ij] ]
        
        MPObjective* const objective = solver->MutableObjective();
        objective->SetMinimization();
        objective->SetCoefficient(x_0,dist_m[0][x_3->solution_value()]);
        objective->SetCoefficient(x_1,dist_m[1][x_0->solution_value()]);
        objective->SetCoefficient(x_2,dist_m[2][x_1->solution_value()]);
        objective->SetCoefficient(x_3,dist_m[3][x_2->solution_value()]);

        LOG(INFO) << objective->Value();
        */




        /*
        for (int i = 1; i < n; i++){
            for (int j = 1; j < i-1; j++){
                objective->SetCoefficient(vec->at(i)->at(j), dist_m[i][j]);
            }
        }
        */
        //constraints
        // ∀(i) 1 <= i <= n     
        // ∀(ij) 1 <= j < i <= n
        // Σ(j<i)[x_ij] + Σ(j>i)[x_ji] = 2

    }

    namespace sat{
        void tsp_cp(){
            CpModelBuilder cp_model;

            //test graph
            //shortest path = 0 -> 1 -> 2 -> 3 -> 0
            int dist_m [4][4] = {
                {int(INFINITY), 1, 1, 4},
                {8, int(INFINITY), 2, 6},
                {9, 7, int(INFINITY), 2},
                {3, 6, 5, int(INFINITY)}
            };
            int n = 4;

            const Domain domain(0.0,1.0);




            
            const IntVar x_0 = cp_model.NewIntVar(domain).WithName("x0");
            const IntVar x_1 = cp_model.NewIntVar(domain).WithName("x1");
            const IntVar x_2 = cp_model.NewIntVar(domain).WithName("x2");
            const IntVar x_3 = cp_model.NewIntVar(domain).WithName("x3");

            IntVar vars[4];
            vars[0] = cp_model.NewIntVar(domain).WithName("x0");
            vars[1] = cp_model.NewIntVar(domain).WithName("x1");
            vars[2] = cp_model.NewIntVar(domain).WithName("x2");
            vars[3] = cp_model.NewIntVar(domain).WithName("x3");

            cp_model.AddAllDifferent(vars);


            
            
            //constraints
            
            


            
        }
    }

    void tsp(){

        int n;

        vector<vector<int> > dist_matrix;
        const int num_vehicles = 1;
        const RoutingIndexManager::NodeIndex depot{0};

        string fileName = "att48_d.txt";    
        ifstream ifile;
        ifile.open(fileName);
        if (!ifile){
            cout << "File name: " << fileName << " does not exist" << endl;
            exit(1);
        }

        ifile >> n;

        
        //dist matrix input

        for(int i = 0; i < n; i++){
            dist_matrix.push_back(vector<int>());
            for(int j = 0; j < n; j++){
                //if (i == j){
                  //  dist_matrix[i].push_back(0);
                    //continue;
                //}
                int k;
                ifile >> k;
                dist_matrix[i].push_back(k);
            }
        }

        /*
       //coordinates
        double f = 0.0, s = 0.0;
        vector<pair<double, double> > coords;
        for (int i = 0; i < n; i++){
        ifile >> f >> s;
        coords.push_back(make_pair(f,s));
        }
        //add edges
        double x1,x2,y1,y2;
        x1 = x2 = y1 = y2 = 0.0;
        //calculates the edge weights for a graph that is euclidean 
        for (int i = 0; i < n; i++){
            dist_matrix.push_back(vector<int>());
            for (int j = 0; j < n; j++){
                if (i == j) {
                    dist_matrix[i].push_back(0);
                    continue;
                }
                x1 = coords[i].first;
                y1 = coords[i].second;
                x2 = coords[j].first;
                y2 = coords[j].second;
                dist_matrix[i].push_back((pow((x2-x1),2) + pow((y2-y1),2)));
            }
        }
        
        */

        RoutingIndexManager manager(n, num_vehicles, depot);

        RoutingModel routing(manager);

        const int transit_callback_index = routing.RegisterTransitCallback([&dist_matrix, &manager](int64_t from_index, int64_t to_index) -> int64_t {
            auto from_node = manager.IndexToNode(from_index).value();
            auto to_node = manager.IndexToNode(to_index).value();
            return dist_matrix[from_node][to_node];
        });

        routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

        RoutingSearchParameters searchParams = DefaultRoutingSearchParameters();
        searchParams.set_first_solution_strategy(FirstSolutionStrategy::PATH_CHEAPEST_ARC);
        //searchParams.set_local_search_metaheuristic(LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
        //searchParams.mutable_time_limit()->set_seconds(30);
        //searchParams.set_log_search(true);

        const Assignment* solution = routing.SolveWithParameters(searchParams);

        LOG(INFO) << "Objective: " << solution->ObjectiveValue() << " miles";
        int64_t index = routing.Start(0);
        LOG(INFO) << "Route: ";
        int64_t distance{0};
        stringstream route;
        while (routing.IsEnd(index) == false){
            route << manager.IndexToNode(index).value() << " -> ";
            int64_t prev = index;
            index = solution->Value(routing.NextVar(index));
            distance += routing.GetArcCostForVehicle(prev, index, int64_t{0});
        }
        LOG(INFO) << route.str() << manager.IndexToNode(index).value();
        LOG(INFO) << "Route distance: " << distance << " miles";
        LOG(INFO) << "Problem solved in " << routing.solver()->wall_time() << "ms";
    }
}


int main() {

    //operations_research::SimpleMipProgram();
    using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::seconds;

    auto t1 = high_resolution_clock::now();

    operations_research::tsp();
	
	auto t2 = high_resolution_clock::now();
	auto s_int = duration_cast<seconds>(t2-t1);
	duration<double, std::milli> ms_double = t2 - t1;
    LOG(INFO) << s_int.count() << "s\n";
    LOG(INFO) << ms_double.count() << "ms\n";


    return 0;
}
