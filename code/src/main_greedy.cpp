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
#include "utils.hpp"
#include "Greedy.hpp"

int main (int argc,char *argv[]) {

    std::string instance_filename, new_solution_filename;
    unsigned int time_limit = 300;

    int tmp;
    if(argc == 1) {
        show_help_info(argv[0]);
        exit(EXIT_FAILURE);
    }

    while((tmp = getopt(argc,argv,"t:p:o:")) != -1) {
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

            case 'o':
                new_solution_filename = optarg;
                break;

            default:
                show_help_info(argv[0]);
                break;
        }
    }

    std::ofstream fout_new_solution(new_solution_filename);
    std::ifstream fin_instance(instance_filename);

    if(!fin_instance.is_open()) {
        std::cerr << "Error al intentar abrir el archivo " << instance_filename << ". " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    //instance model
    Instance *instance = new Instance();
    instance->read_instance_from_file(fin_instance);
    //original solution
    Solution *solution = new Solution(instance);

    instance->init();

    Greedy *greedy = new Greedy(instance, solution);
    greedy->set_time_limit(time_limit);
    greedy->run();

    if(greedy->get_num_assigned_processes() == instance->processes.size()) {
        solution->update_solution_costs();
        std::cout << solution->get_solution_cost() << ";";
    } else {
        std::cout << "-" << ";";
    }

    std::cout << greedy->get_execution_time() << ";" << instance->processes.size() << ";" << greedy->get_num_assigned_processes() << std::endl;

    delete instance;
    delete solution;
    delete greedy;

    fout_new_solution.close();
    fin_instance.close();

    return 0;
}
