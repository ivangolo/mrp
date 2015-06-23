#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <deque>
#include "utils.h"
#include <algorithm>
#include <memory>
#include "Instance.h"
#include <iterator>
#include "Resource.h"
#include "Machine.h"
#include "Service.h"
#include "Process.h"
#include "Balance.h"
#include "sets.h"

using namespace std;
int main (int argc,char *argv[]) {

    int time_limit, seed;
    string instance_filename, original_solution_filename, new_solution_filename;

    char tmp;
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
    fout_new_solution << "Something new" << endl;
    fout_new_solution.close();

    ifstream fin_instance(instance_filename);
    ifstream fin_original_solution(original_solution_filename);

    if(!fin_instance.is_open()) {
        cerr << "Error al intentar abrir el archivo " << instance_filename << endl;
        exit(EXIT_FAILURE);
    } else if(!fin_original_solution.is_open()) {
        cerr << "Error al intentar abrir el archivo " << instance_filename << endl;
        exit(EXIT_FAILURE);
    }

    //original solution
    deque<unsigned int> original_solution;
    get_original_solution(fin_original_solution, original_solution);

    //instance model
    Instance *instance = new Instance();
    instance->read_instance(fin_instance);

    cout << "Number of resources: " << instance->resources.size() << endl;
    cout << "Number of machines: " << instance->machines.size() << endl;
    cout << "Number of services: " << instance->services.size() << endl;
    cout << "Number of processes: " << instance->processes.size() << endl;
    cout << "Instances weights:" << endl;
    cout << instance->weight_process_move_cost << endl;
    cout << instance->weight_machine_move_cost << endl;
    cout << instance->weight_service_move_cost << endl;

    delete instance;

    fin_instance.close();
    fin_original_solution.close();

    /*
    deque<Balance*>::iterator balance_iterator;
    for (balance_iterator = balances.begin(); balance_iterator != balances.end(); ++balance_iterator) {
        (*balance_iterator)->print();
    }
     */



    //show solution
   /* std::copy(original_solution.begin(), original_solution.end(), std::ostream_iterator<unsigned int>(std::cout, " "));
    cout << endl;*/

    cout << "Time limit: " << time_limit << endl;
    cout << "Instance Filename: " << instance_filename << endl;
    cout << "Original Solution filename: " << original_solution_filename << endl;
    cout << "New Solution filename: " << new_solution_filename << endl;
    cout << "Seed: " << seed << endl;


    return 0;
}