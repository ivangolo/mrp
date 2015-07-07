//
// Created by ivanedo on 12-06-15.
//

#include <iostream>
#include "utils.h"
#include <fstream>

void show_help_info(char *s) {
    std::cout << "Usage:   " << s << " [-option] [argument]" << std::endl;
    std::cout << "option:  " << "-t time_limit" << std::endl;
    std::cout << "         " << "-p path/to/instance_filename" << std::endl;
    std::cout << "         " << "-i path/to/original_solution_filename" << std::endl;
    std::cout << "         " << "-o path/to/new_solution_filename" << std::endl;
    std::cout << "         " << "-s seed" << std::endl;
    std::cout << "example: " << s << " -t 300 -p model_a1_1.txt -i assignment_a1_1.txt -o salida.txt -s 100" << std::endl;
}


