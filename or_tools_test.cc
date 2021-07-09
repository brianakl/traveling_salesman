#include <iostream>
#include <fstream>
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/sat/cp_model.h"
#include <vector>
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

    struct DataModel{
        /*
        const vector<vector<int> > dist_matrix{
            {int(INFINITY), 1, 1, 4},
            {8, int(INFINITY), 2, 6},
            {9, 7, int(INFINITY), 2},
            {3, 6, 5, int(INFINITY)}
        };
        const vector<vector<int> > dist_matrix{
            {0,	66,	9, 85, 7, 47, 18, 45, 45, 78, 23},
            {5,	0,	81, 98,	45, 43, 45, 36, 82, 89, 22},
            {71, 92, 0, 43, 77, 93, 74, 71, 23, 17, 90},
            {40, 49, 18, 0, 32, 88, 56, 52, 93, 1, 42},
            {40, 37, 67, 58, 0, 47, 89, 21, 20, 14, 53},
            {4,	37,	9, 11, 98, 0, 9, 42, 1, 85, 12},
            {3,	66, 8, 43, 50, 98, 0, 29, 19, 66, 61},
            {48, 48, 3,	81,	95,	28, 60, 0, 47, 30, 39},
            {100, 17, 75, 49, 79, 79, 42, 0, 29, 49},
            {44, 13,	59,	15,	99,	75,	53,	47, 0, 82},
            {10,2,	71,	99,	80,	35,	65,	76, 79, 0}
        };
       

        vector<vector<int> > dist_matrix {
            {0, 29, 82, 46, 68, 52, 72, 42, 51, 55, 29, 74, 23, 72, 46 },
            {29,  0, 55, 46, 42, 43, 43, 23, 23, 31, 41, 51, 11, 52, 21 },
            {82, 55,  0, 68, 46, 55, 23, 43, 41, 29, 79, 21, 64, 31, 51 },
            {46, 46, 68,  0, 82, 15, 72, 31, 62, 42, 21, 51, 51, 43, 64 },
            {68, 42, 46, 82,  0, 74, 23, 52, 21, 46, 82, 58, 46, 65, 23 },
            {52, 43, 55, 15, 74,  0, 61, 23, 55, 31, 33, 37, 51, 29, 59 },
            {72, 43, 23, 72, 23, 61,  0, 42, 23, 31, 77, 37, 51, 46, 33 },
            {42, 23, 43, 31, 52, 23, 42,  0, 33, 15, 37, 33, 33, 31, 37 },
            {51, 23, 41, 62, 21, 55, 23, 33,  0, 29, 62, 46, 29, 51, 11 },
            {55, 31, 29, 42, 46, 31, 31, 15, 29,  0, 51, 21, 41, 23, 37 },
            {29, 41, 79, 21, 82, 33, 77, 37, 62, 51,  0, 65, 42, 59, 61 },
            {74, 51, 21, 51, 58, 37, 37, 33, 46, 21, 65,  0, 61, 11, 55 },
            {23, 11, 64, 51, 46, 51, 51, 33, 29, 41, 42, 61,  0, 62, 23 },
            {72, 52, 31, 43, 65, 29, 46, 31, 51, 23, 59, 11, 62,  0, 59 },
            {46, 21, 51, 64, 23, 59, 33, 37, 11, 37, 61, 55, 23, 59,  0},
            };
        */

        const int num_vehicles = 1;
        const RoutingIndexManager::NodeIndex depot{0};
        
    };

    void tsp(){
        //DataModel data;

        int n;

        vector<vector<int> > dist_matrix;
        const int num_vehicles = 1;
        const RoutingIndexManager::NodeIndex depot{0};

        string fileName = "1000_tsp.txt";    
        ifstream ifile;
        ifile.open(fileName);
        if (!ifile){
            cout << "File name: " << fileName << " does not exist" << endl;
            exit(1);
        }

        ifile >> n;

        
        //dist matrix input
        /*

        for(int i = 0; i < n; i++){
            dist_matrix.push_back(vector<int>());
            for(int j = 0; j < n; j++){
                if (i == j){
                    dist_matrix[i].push_back(0);
                    continue;
                }
                int k;
                ifile >> k;
                dist_matrix[i].push_back(k);
            }
        }
        */

       //coordinates
        double f = 0, s = 0;
        vector<pair<double, double> > coords;
        for (int i = 0; i < n; i++){
        ifile >> f >> s;
        coords.push_back(make_pair(f,s));
        }
        //add edges
        double x1,x2,y1,y2;
        x1 = x2 = y1 = y2 = 0.0;
        //calculates the edge weights for a graph that is euclidean 
        for (int i = 1; i < n; i++){
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

    operations_research::tsp();

    return 0;
}
