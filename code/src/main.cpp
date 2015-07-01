#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <deque>
#include <algorithm>
#include <memory>
#include <time.h>
#include "Instance.h"
#include "Solution.h"
#include "utils.h"
using namespace std;
int main (int argc,char *argv[]) {

    int seed;
    string instance_filename, original_solution_filename, new_solution_filename;
    unsigned int time_limit;

    int tmp;
    if(argc == 1) {
        show_help_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    while((tmp = getopt(argc,argv,"t:p:i:o:s:")) != -1) {
        switch(tmp) {
            case 't': {
                istringstream iss_tm(optarg);
                if (!(iss_tm >> time_limit)) {
                    cerr << "Número inválido " << optarg << endl;
                    exit(EXIT_FAILURE);
                }
                break;

            }
            case 'p':
                instance_filename = optarg;
                break;

            case 'i':
                original_solution_filename = optarg;
                break;

            case 'o':
                new_solution_filename = optarg;
                break;

            case 's': {
                istringstream iss_seed(optarg);
                if (!(iss_seed >> seed)) {
                    cerr << "Número inválido " << optarg << endl;
                    exit(EXIT_FAILURE);
                }
                break;
            }

            default:
                show_help_info(argv[0]);
                break;
        }
    }


    ofstream fout_new_solution(new_solution_filename);

    ifstream fin_instance(instance_filename);
    ifstream fin_original_solution(original_solution_filename);

    if(!fin_instance.is_open()) {
        cerr << "Error al intentar abrir el archivo " << instance_filename << endl;
        exit(EXIT_FAILURE);
    } else if(!fin_original_solution.is_open()) {
        cerr << "Error al intentar abrir el archivo " << instance_filename << endl;
        exit(EXIT_FAILURE);
    }

    //instance model
    Instance *instance = new Instance();
    instance->read_instance_from_file(fin_instance);

    //original solution
    Solution *original_solution = new Solution(instance);
    original_solution->read_solution_from_file(fin_original_solution);

    instance->init(original_solution->get_assignments());
    original_solution->update_solution_costs();
    original_solution->print();
    cout << "Costo solución original: " << original_solution->get_solution_cost() << endl;

    /*
    cout << "Spread: " << std::boolalpha << original_solution->check_all_spread_constraints() << endl;
    cout << "Capacity: " << std::boolalpha << original_solution->check_all_capacity_constraints() << endl;
    cout << "Conflict: " << std::boolalpha << original_solution->check_all_conflict_constraints() << endl;
    cout << "Dependency: " << std::boolalpha << original_solution->check_all_dependency_constraints() << endl;
    */
    /*
    clock_t endwait;
    endwait = clock() + time_limit * CLOCKS_PER_SEC;
    while(clock() < endwait) {

    }
     */
    bool found = false;
    for (unsigned int i = 0; !found && i < instance->processes.size(); ++i) {
        for (unsigned int j = 0; !found && j < instance->machines.size(); ++j) {

            if(original_solution->get_current_assignment(i) != j) {
                if(original_solution->check_shift(i, j)) {
                    original_solution->shift_process(i,j);
                    cout << "Proceso: " << i << endl;
                    cout << "Máquina: " << j << endl;
                    found = true;
                }
            }
        }
    }

    cout << "Factible: " << std::boolalpha << original_solution->check_solution() << endl;
    original_solution->print();
    cout << "Costo nueva solución: " << original_solution->get_solution_cost() << endl;
    original_solution->write_solution_to_file(fout_new_solution);


    delete instance;
    delete original_solution;

    fout_new_solution.close();
    fin_instance.close();
    fin_original_solution.close();

    cout << "Time limit: " << time_limit << endl;
    cout << "Instance Filename: " << instance_filename << endl;
    cout << "Original Solution filename: " << original_solution_filename << endl;
    cout << "New Solution filename: " << new_solution_filename << endl;
    cout << "Seed: " << seed << endl;


    return 0;
}