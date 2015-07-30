//
// Created by ivanedo on 29-07-15.
//

#include "Greedy.h"
#include "utils.h"

Greedy::Greedy() {

}

Greedy::Greedy(Instance *instance, Solution *solution) {
    this->instance = instance;
    this->solution = solution;
    this->assigned_processes = 0;
}

Greedy::~Greedy() {

}

Solution *Greedy::run() {

    instance->classify_services();
    instance->sort_services_by_dependencies();

    for (ProcessList::iterator process_id = instance->less_restricted_processes.begin(); process_id != instance->less_restricted_processes.end(); ++process_id) {
        SolutionNeighborhood neighborhood;
        Process *process = instance->get_process(*process_id);
        if(process->is_assigned()) {
            continue;
        }
        for (MachineList::iterator machine_id = instance->sorted_machines.begin(); machine_id != instance->sorted_machines.end(); ++machine_id) {
            if(solution->relaxed_check_assignment(process->get_id(), *machine_id)) {
                int64_t load_cost = solution->get_load_cost_with_process(process->get_id(), *machine_id);
                neighborhood[*machine_id] = load_cost;
            }
        }

        if(!neighborhood.empty()) {
            std::pair<unsigned int, int64_t> best_machine = get_min(neighborhood);
            solution->assign_process(process->get_id(), best_machine.first);
            process->set_assigned_status(true);
            assigned_processes++;
        }
    }

    unsigned int i;
    do {
        for (ProcessList::iterator process_id = instance->restricted_processes.begin(); process_id != instance->restricted_processes.end(); ++process_id) {
            SolutionNeighborhood neighborhood;
            Process *process = instance->get_process(*process_id);
            if(process->is_assigned()) {
                continue;
            }
            for (MachineList::iterator machine_id = instance->sorted_machines.begin(); machine_id != instance->sorted_machines.end(); ++machine_id) {
                if(solution->check_assignment(process->get_id(), *machine_id)) {
                    int64_t load_cost = solution->get_load_cost_with_process(process->get_id(), *machine_id);
                    neighborhood[*machine_id] = load_cost;
                }
            }

            if(!neighborhood.empty()) {
                std::pair<unsigned int, int64_t> best_machine = get_min(neighborhood);
                solution->assign_process(process->get_id(), best_machine.first);
                process->set_assigned_status(true);
                assigned_processes++;
            }
        }
        i++;
    } while(i < 2);

    return solution;
}


unsigned int Greedy::get_num_assigned_processes() {
    return assigned_processes;
}
