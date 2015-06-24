//
// Created by ivanedo on 23-06-15.
//

#include "Costs.h"


unsigned int Costs::load_cost(Instance &instance, unsigned int machine_id, unsigned int resource_id) {
    unsigned int usage = instance.machines.at(machine_id)->usages.at(resource_id);
    unsigned int scapacity = instance.machines.at(machine_id)->safety_capacities.at(resource_id);

    return (usage > scapacity) ? (usage - scapacity) : 0;
}

unsigned int Costs::balance_cost(Instance &instance, unsigned int machine_id, unsigned int balance_id) {
    Machine *machine = instance.machines.at(machine_id);
    Balance *balance = instance.balances.at(balance_id);

    unsigned int capacity_r1 = machine->capacities.at(balance->get_resource_id(1));
    unsigned int usage_r1 = machine->usages.at(balance->get_resource_id(1));
    unsigned int available_r1 = capacity_r1 - usage_r1;

    unsigned int capacity_r2 = machine->capacities.at(balance->get_resource_id(2));
    unsigned int usage_r2 = machine->usages.at(balance->get_resource_id(2));
    unsigned int available_r2 = capacity_r2 - usage_r2;

    unsigned int target = balance->get_target();

    return ((target * available_r1 - available_r2) > 0) ? (target * available_r1 - available_r2) : 0;
}

unsigned int Costs::process_move_cost(Instance &instance, unsigned int process_id) {
    Process *process = instance.processes.at(process_id);
    return (process->get_initial_machine_id() != process->get_current_machine_id()) ? process->get_process_move_cost() * instance.weight_process_move_cost : 0;
}

unsigned int Costs::service_move_cost(Instance &instance, unsigned int service_id) {
    unsigned int num_moved_processes = 0;
    Service *service = instance.services.at(service_id);
    for(std::deque<unsigned int>::iterator it = service->processes.begin(); it != service->processes.end(); ++it) {
        unsigned int initial_machine = instance.processes.at(*it)->get_initial_machine_id();
        unsigned int current_machine = instance.processes.at(*it)->get_current_machine_id();
        if(initial_machine != current_machine) {
            num_moved_processes += 1;
        }
    }
    return num_moved_processes * instance.weight_service_move_cost;
}

unsigned int Costs::machine_move_cost(Instance &instance, unsigned int process_id) {
    Process *process = instance.processes.at(process_id);
    unsigned int initial_machine_id = process->get_initial_machine_id();
    unsigned int current_machine_id = process->get_current_machine_id();

    if(initial_machine_id == current_machine_id) {
        return 0;
    } else {
        Machine *initial_machine = instance.machines.at(initial_machine_id);
        return initial_machine->mmc.at(current_machine_id) * instance.weight_machine_move_cost;
    }
}

unsigned int Costs::machine_load_cost(Instance &instance, unsigned int machine_id) {
    unsigned int cost = 0;
    std::deque<Resource*>::iterator resource_iter;
    for (resource_iter = instance.resources.begin(); resource_iter != instance.resources.end(); ++resource_iter) {
        cost += Costs::load_cost(instance, machine_id, (*resource_iter)->get_id()) * (*resource_iter)->get_weight_load_cost();
    }
    return cost;
}


unsigned int Costs::machine_balance_cost(Instance &instance, unsigned machine_id) {
    unsigned int cost = 0;
    std::deque<Balance*>::iterator balance_iter;
    for (balance_iter = instance.balances.begin(); balance_iter != instance.balances.end(); ++balance_iter) {
        cost += Costs::balance_cost(instance, machine_id, (*balance_iter)->get_id()) * (*balance_iter)->get_weight_balance_cost();
    }
    return cost;
}


unsigned int Costs::machine_cost(Instance &instance, unsigned int machine_id) {
    return Costs::machine_load_cost(instance, machine_id) + Costs::machine_balance_cost(instance, machine_id);
}


unsigned int Costs::process_cost(Instance &instance, unsigned int process_id) {
    return Costs::process_move_cost(instance, process_id) + Costs::machine_move_cost(instance, process_id);
}


unsigned int Costs::total_machine_cost(Instance &instance) {
    unsigned int cost = 0;
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = instance.machines.begin(); machine_iter != instance.machines.end(); ++machine_iter) {
        cost += Costs::machine_cost(instance, (*machine_iter)->get_id());
    }
    return cost;
}

unsigned int Costs::total_process_cost(Instance &instance) {
    unsigned int cost = 0;
    std::deque<Process*>::iterator process_iter;
    for (process_iter = instance.processes.begin(); process_iter != instance.processes.end(); ++process_iter) {
        cost += Costs::process_cost(instance, (*process_iter)->get_id());
    }
    return cost;
}


unsigned int Costs::total_service_cost(Instance &instance) {
    unsigned int max = 0;
    std::deque<Service*>::iterator service_iter;
    for (service_iter = instance.services.begin(); service_iter != instance.services.end(); ++service_iter) {
        unsigned int cost = Costs::service_move_cost(instance, (*service_iter)->get_id());
        if(cost > max) {
            max = cost;
        }
    }
    return max;
}
