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
#include "Instance.hpp"
#include "Solution.hpp"
#include "HillClimbing.hpp"
#include "utils.hpp"
#include "Greedy.hpp"

int main (int argc,char *argv[]) {

    std::string instance_filename, original_solution_filename, new_solution_filename;
    unsigned int time_limit = 300, seed = 0;
    //HillClimbing::Mode process_selection_mode = HillClimbing::INSTANCE_SEQUENCE;

    int tmp;
    if(argc == 1) {
        show_help_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    while((tmp = getopt(argc,argv,"t:p:i:o:s:a:")) != -1) {
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

            case 'a': {
                std::string mode(optarg);
                if(!mode.empty() && mode == "sorted") {
                    //process_selection_mode = HillClimbing::SORTED_BY_SIZE;
                } else if(!mode.empty() && mode != "sorted") {
                    std::cerr << "Hill Climbing mode inválido " << optarg << std::endl;
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
    Instance *instance = new Instance();
    instance->read_instance_from_file(fin_instance);
    //original solution
    Solution *solution = new Solution(instance);

    /*
    solution->read_solution_from_file(fin_original_solution);


    if(instance->processes.size() != solution->get_assignments().size()) {
        std::cerr << "Número de procesos de la instancia y de la solución inicial no concuerdan"  << std::endl;
        delete instance;
        delete solution;
        exit(EXIT_FAILURE);
    }


    //instance->add_assignments(solution->get_assignments());
    //instance->update_all_usages();
     */

    instance->init();

    Greedy *greedy = new Greedy(instance, solution);
    greedy->set_time_limit(time_limit);
    greedy->run();
    std::cout << instance_filename << std::endl;
    greedy->print();

    /*
    solution->update_solution_costs();

    std::cout << "-.initial_assignment_costs::" << std::endl;
    solution->print();

    HillClimbing *hc = new HillClimbing(instance, solution);
    hc->set_time_limit(time_limit);
    hc->set_process_selection_mode(process_selection_mode);
    hc->run();

    std::cout << "-.new_assignment_costs::" << std::endl;
    solution->print();

    hc->print();
    solution->write_solution_to_file(fout_new_solution);
    */

    delete instance;
    delete solution;
    delete greedy;

    fout_new_solution.close();
    fin_instance.close();
    fin_original_solution.close();

    return 0;
}
