#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <deque>
#include <algorithm>
#include <memory>
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

    if(instance->processes.size() != original_solution->get_assignments().size()) {
        delete instance;
        delete original_solution;
        cerr << "Número de procesos de la instancia y de la solución inicial no concuerdan"  << endl;
        exit(EXIT_FAILURE);
    }

    instance->init(original_solution->get_assignments());
    original_solution->update_solution_costs();
    original_solution->print();
    cout << "Costo solución original: " << original_solution->get_solution_cost() << endl;

    unsigned int times = 0;
    bool changes;

    do {
        changes = false;
        times++;
        for (unsigned int process_id = 0; process_id < instance->processes.size(); ++process_id) {

            SolutionNeighborhood neighborhood; //<machine_id, delta_cost>
            unsigned int current_process_assignment = original_solution->get_current_assignment(process_id);

            //generate the neighborhood
            for (unsigned int machine_id = 0; machine_id < instance->machines.size(); ++machine_id) {

                if (machine_id != current_process_assignment) {

                    if (original_solution->check_shift(process_id, machine_id)) { //only feasible solutions
                        int64_t delta_cost = original_solution->calc_delta_cost_with_shift(process_id, machine_id);

                        if (delta_cost < 0) {  //only solutions that improves the current solution
                            neighborhood[machine_id] = delta_cost;
                        }
                    }
                }
            }

            //check the best shift
            if (!neighborhood.empty()) {
                std::pair<unsigned int, int64_t> best_machine = get_min(neighborhood);
                original_solution->shift_process(process_id, best_machine.first);
                changes = true;
            }
        }

    } while(changes);

    original_solution->print();
    cout << "Costo nueva solución: " << original_solution->get_solution_cost() << endl;
    cout << "Iteraciones do-while: " << times << endl;
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