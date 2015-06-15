#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "utils.h"

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

    string line;
    getline(fin_instance, line);
    cout << line << endl;

    fin_instance.close();
    fin_original_solution.close();



    cout << "Time limit: " << time_limit << endl;
    cout << "Instance Filename: " << instance_filename << endl;
    cout << "Original Solution filename: " << original_solution_filename << endl;
    cout << "New Solution filename: " << new_solution_filename << endl;
    cout << "Seed: " << seed << endl;


    return 0;
}