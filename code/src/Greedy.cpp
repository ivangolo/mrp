//
// Created by ivanedo on 29-07-15.
//

#include "Greedy.hpp"
#include <algorithm>
#include <iostream>

Greedy::Greedy(Instance *instance, Solution *solution): LocalSearch(instance, solution), assigned_processes(0) {}

Greedy::~Greedy() {}

Solution *Greedy::run() {

    time_t start_time = time(NULL);

    for (ProcessList::iterator process_id = instance->less_restricted_processes.begin(); process_id != instance->less_restricted_processes.end(); ++process_id) {
        Process *process = instance->get_process(*process_id);
        for (unsigned int machine_id = 0; machine_id != instance->machines.size(); ++machine_id) {
            if(solution->relaxed_check_assignment(process->get_id(), machine_id)) {
                int64_t load_cost = solution->get_load_cost_with_process(process->get_id(), machine_id);
                add_neighbour(machine_id, load_cost);
            }
        }

        if(!neighborhood.empty()) {
            unsigned int best_machine = get_min_machine();
            solution->assign_process(process->get_id(), best_machine);
            process->set_assigned_status(true);
            neighborhood.clear();
            assigned_processes++;
        }

        execution_time = difftime(time(NULL), start_time);
        if(execution_time >= time_limit) {
            break;
        }
    }

    neighborhood.clear();

    for (ServiceList::iterator service_id = instance->restricted_services.begin(); execution_time < time_limit && service_id != instance->restricted_services.end(); ++service_id) {
        Service *service = instance->get_service(*service_id);

        for (ProcessList::iterator process_id = service->processes.begin(); process_id != service->processes.end(); ++process_id) {
            Process *process = instance->get_process(*process_id);

            if(!process->is_assigned()) {
                for (unsigned int machine_id = 0; machine_id < instance->machines.size(); ++machine_id) {
                    if(solution->check_assignment(process->get_id(), machine_id)) {
                        int64_t load_cost = solution->get_load_cost_with_process(process->get_id(), machine_id);
                        add_neighbour(machine_id, load_cost);
                    }

                }

                if(!neighborhood.empty()) {
                    unsigned int best_machine = get_min_machine();
                    solution->assign_process(process->get_id(), best_machine);
                    process->set_assigned_status(true);
                    neighborhood.clear();
                    assigned_processes++;
                }
            }

            execution_time = difftime(time(NULL), start_time);
            if(execution_time >= time_limit) {
                break;
            }
        }

    }

    return solution;
}


unsigned int Greedy::get_num_assigned_processes() {
    return assigned_processes;
}

void Greedy::print() {
    std::cout << "Total processes: " << instance->processes.size() << std::endl;
    std::cout << "Total assigned processes: " << get_num_assigned_processes() << std::endl;
    std::cout << "Execution time: " << get_execution_time() << std::endl;
}

void Greedy::set_time_limit(unsigned int time_limit) {
    LocalSearch::set_time_limit(time_limit);
}


double Greedy::get_execution_time() {
    return LocalSearch::get_execution_time();
}
