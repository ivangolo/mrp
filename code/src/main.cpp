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

    std::string instance_filename, original_solution_filename, new_solution_filename;
    unsigned int time_limit = 300, seed = 0;

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

    instance->init(original_solution->get_assignments());
    original_solution->update_solution_costs();

    std::cout << "-.initial_assignment_costs::" << std::endl;
    original_solution->print();

    HillClimbing *hc = new HillClimbing(instance, original_solution);
    hc->set_time_limit(time_limit);
    hc->run(true);

    std::cout << "-.new_assignment_costs::" << std::endl;
    original_solution->print();

    hc->print();
    original_solution->write_solution_to_file(fout_new_solution);

    delete instance;
    delete original_solution;
    delete hc;

    fout_new_solution.close();
    fin_instance.close();
    fin_original_solution.close();

    return 0;
}
