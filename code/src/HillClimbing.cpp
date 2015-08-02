//
// Created by ivanedo on 21-07-15.
//

#include "HillClimbing.h"
#include <iostream>
#include <algorithm>

HillClimbing::HillClimbing() {

}

HillClimbing::HillClimbing(Instance *instance, Solution *solution) {
    this->instance = instance;
    this->solution = solution;
    this->num_iterations = 0;
    this->execution_time = 0;
}

HillClimbing::~HillClimbing() {

}

Solution * HillClimbing::run() {

    bool changes;
    time_t start_time = time(NULL);

    do {
        changes = false;
        for(unsigned int i = 0; i < instance->processes.size(); ++i) {
            unsigned int process_id = (mode == HillClimbing::SORTED_BY_SIZE) ? instance->sorted_processes[i] : i;
            unsigned int current_process_assignment = instance->get_process(process_id)->get_current_machine_id();

            //generate the neighborhood
            for (unsigned int machine_id = 0; machine_id < instance->machines.size(); ++machine_id) {

                if (machine_id != current_process_assignment) {

                    if (solution->check_shift(process_id, machine_id)) { //only feasible solutions
                        int64_t delta_cost = solution->calc_delta_cost_with_shift(process_id, machine_id);

                        if (delta_cost < 0) {  //only solutions that improves the current solution
                            add_neighbour(machine_id, delta_cost);
                        }
                    }
                }

            }

            //check the best shift
            if (!neighborhood.empty()) {
                std::pair<unsigned int, int64_t> best_machine = get_min_shift();
                solution->shift_process(process_id, best_machine.first);
                changes = true;
                neighborhood.clear();
            }

            num_iterations++;

            // updating the execution time
            execution_time = difftime(time(NULL), start_time);
            if(execution_time >= time_limit) {
                break;
            }

        }

    } while(execution_time < time_limit && changes);


    return this->solution;
}


void HillClimbing::set_time_limit(unsigned int time_limit) {
    this->time_limit = time_limit;
}


double HillClimbing::get_execution_time() {
    return execution_time;
}

int32_t HillClimbing::get_num_iterations() {
    return num_iterations;
}


void HillClimbing::print() {
    std::cout << "-.algorithm_stats::" << std::endl;
    std::cout << "iterations: " << get_num_iterations() <<  ", ";
    std::cout << "running_time: " << get_execution_time() << std::endl;
}


void HillClimbing::add_neighbour(unsigned int machine_id, int64_t cost_decrement) {
    neighborhood[machine_id] = cost_decrement;
}

std::pair<unsigned int, int64_t> HillClimbing::get_min_shift() {
    return *min_element(neighborhood.begin(), neighborhood.end(), ShiftMinCost());
}


void HillClimbing::set_process_selection_mode(Mode mode) {
    this->mode = mode;
}
