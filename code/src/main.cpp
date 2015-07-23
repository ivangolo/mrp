#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <deque>
#include <queue>
#include <algorithm>
#include <ctime>
#include <memory>
#include <cstring>
#include "Instance.h"
#include "Solution.h"
#include "HillClimbing.h"
#include "utils.h"

int main (int argc,char *argv[]) {

    int seed = 0;
    std::string instance_filename, original_solution_filename, new_solution_filename;
    unsigned int time_limit = 300;

    int tmp;
    if(argc == 1) {
        show_help_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    while((tmp = getopt(argc,argv,"t:p:i:o:s:")) != -1) {
        switch(tmp) {
            case 't': {
                std::istringstream iss_tm(optarg);
                if (!(iss_tm >> time_limit)) {
                    std::cerr << "Número inválido " << optarg << std::endl;
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
                std::istringstream iss_seed(optarg);
                if (!(iss_seed >> seed)) {
                    std::cerr << "Número inválido " << optarg << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            }

            default:
                show_help_info(argv[0]);
                break;
        }
    }


    std::ofstream fout_new_solution(new_solution_filename);
    std::ifstream fin_instance(instance_filename);
    std::ifstream fin_original_solution(original_solution_filename);


    if(!fin_instance.is_open()) {
        std::cerr << "Error al intentar abrir el archivo " << instance_filename << ". " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else if(!fin_original_solution.is_open()) {
        std::cerr << "Error al intentar abrir el archivo " << original_solution_filename << ". " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    //instance model
    Instance *instance = new Instance(fin_instance);

    //original solution
    Solution *original_solution = new Solution(instance, fin_original_solution);

    if(instance->processes.size() != original_solution->get_assignments().size()) {
        std::cerr << "Número de procesos de la instancia y de la solución inicial no concuerdan"  << std::endl;
        delete instance;
        delete original_solution;
        exit(EXIT_FAILURE);
    }


    /*
    Solution *greedy_solution = new Solution(instance);



    unsigned int num_of_assigned_processes = 0;
    do {
        //std::cout << "Proceso: " << processes.front().first << ", costo: " << processes.front().second << std::endl;
        for (unsigned int i = 0; i < processes.size(); ++i) {
            unsigned int process_id = processes[i].first;
            if(greedy_solution->get_current_assignment(process_id) == 60000) {
                for (unsigned int machine_id = 0; machine_id < instance->machines.size(); ++machine_id) {

                    if (greedy_solution->check_assignment(process_id, machine_id)) { //only feasible solutions
                        greedy_solution->assign_process(process_id, machine_id);
                        std::cout << "Proceso " << process_id << " asignado" << std::endl;
                        num_of_assigned_processes++;
                        break;
                    } else {
                        std::cout << "Proceso " << process_id << " no asignado" << std::endl;
                    }
                }
            }
        }

    } while(num_of_assigned_processes <= instance->get_num_of_processes());



    //greedy_solution->print_assignments();
    //std::cout << "numero procesos con greedy: " << greedy_solution->get_assignments().size() << std::endl;
   // std::cout << "numero procesos con original: " << original_solution->get_assignments().size() << std::endl;

    //greedy_solution->write_solution_to_file(fout_new_solution);

     */

    instance->init(original_solution->get_assignments());
    original_solution->update_solution_costs();

    std::cout << "-.initial_assignment_costs::" << std::endl;
    original_solution->print();

    HillClimbing *hc = new HillClimbing(instance, original_solution);
    hc->set_time_limit(time_limit);
    hc->run(true);

    std::cout << "-.new_assignment_costs::" << std::endl;
    original_solution->print();

    std::cout << "-.algorithm_stats::" << std::endl;
    std::cout << "iterations: " << hc->get_num_iterations() <<  ", ";
    std::cout << "running_time: " << hc->get_execution_time() << std::endl;
    original_solution->write_solution_to_file(fout_new_solution);


    delete instance;
    delete original_solution;
    delete hc;

    fout_new_solution.close();
    fin_instance.close();
    fin_original_solution.close();

    /*
    std::cout << std::endl << "arguments:" << std::endl;
    std::cout << "time_limit: " << time_limit << std::endl;
    std::cout << "model_filename: " << instance_filename << std::endl;
    std::cout << "initial_assignment_filename: " << original_solution_filename << std::endl;
    std::cout << "new_assignment_filename: " << new_solution_filename << std::endl;
    std::cout << "seed: " << seed << std::endl;
    */


    return 0;
}
