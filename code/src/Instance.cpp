//
// Created by ivanedo on 13-06-15.
//

#include "Instance.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>

Instance::Instance() {}

Instance::~Instance() {
    std::for_each(resources.begin(), resources.end(), std::default_delete<Resource>());
    std::for_each(machines.begin(), machines.end(), std::default_delete<Machine>());
    std::for_each(services.begin(), services.end(), std::default_delete<Service>());
    std::for_each(processes.begin(), processes.end(), std::default_delete<Process>());
    std::for_each(balances.begin(), balances.end(), std::default_delete<Balance>());
}

void Instance::read_instance_from_file(std::ifstream &fin_instance) {

    //reading resources
    unsigned int num_resources;
    fin_instance >> num_resources;
    for(unsigned int i = 0; i < num_resources; ++i) {
        unsigned int transient;
        unsigned int weight;
        fin_instance >> transient;
        fin_instance >> weight;

        add_resource(new Resource(i, weight, (bool)transient));
    }

    // reading machines
    unsigned int num_machines;
    fin_instance >> num_machines;
    for(unsigned int i = 0; i < num_machines; ++i) {
        unsigned int neighborhood, location, capacity, safety_capacity, move_cost;
        fin_instance >> neighborhood;
        fin_instance >> location;

        Machine *machine = new Machine(i, neighborhood, location);

        //reading resource capacities
        for (unsigned int j = 0; j < num_resources; ++j) {
            fin_instance >> capacity;
            machine->add_capacity(capacity);
        }
        //reading resource safety_capacities
        for (unsigned int k = 0; k < num_resources; ++k) {
            fin_instance >> safety_capacity;
            machine->add_safety_capacity(safety_capacity);
        }
        //reading machine move costs
        for (unsigned int l = 0; l < num_machines; ++l) {
            fin_instance >> move_cost;
            machine->add_machine_move_cost(move_cost);
        }
        //initialize usages
        machine->init_usages(num_resources);
        add_machine(machine);
    }

    //reading services
    unsigned int num_services;
    fin_instance >> num_services;
    for(unsigned int m = 0; m < num_services; ++m) {
        unsigned int spread_min, num_dependencies, service_id;
        fin_instance >> spread_min;
        fin_instance >> num_dependencies;

        Service *service = new Service(m, spread_min);

        if(num_dependencies > 0) {
            for(unsigned int i = 0; i < num_dependencies; ++i) {
                fin_instance >> service_id;
                service->add_dependency(service_id);
            }
            add_service(service);

        } else {//if the service hasn't dependencies
            add_service(service);
            continue;
        }
    }

    //reading processes
    unsigned int num_processes;
    fin_instance >> num_processes;
    for (unsigned int n = 0; n < num_processes; ++n) {
        unsigned int service_id, requirement, process_move_cost;
        fin_instance >> service_id;
        Process *process = new Process(n, service_id);

        //reading resource requirements
        for (unsigned int i = 0; i < num_resources; ++i) {
            fin_instance >> requirement;
            process->add_requirement(requirement);
            process->increment_process_size(requirement);
        }
        // process move cost
        fin_instance >> process_move_cost;
        process->set_process_move_cost(process_move_cost);

        //adding the process to the service owner
        get_service(service_id)->add_process(n);

        sorted_processes.push_back(n);
        add_process(process);
    }

    //reading balances
    unsigned int num_balances;
    fin_instance >> num_balances;
    for (unsigned int i = 0; i < num_balances; ++i) {
        unsigned int resource1, resource2, target, weight;
        fin_instance >> resource1;
        fin_instance >> resource2;
        fin_instance >> target;
        fin_instance >> weight;

        add_balance(new Balance(i, target, resource1, resource2, weight));

    }

    //reading final weights
    fin_instance >> weight_process_move_cost;
    fin_instance >> weight_service_move_cost;
    fin_instance >> weight_machine_move_cost;

}

void Instance::add_assignments(Assignments assignments) {
    for(unsigned int process_id = 0; process_id < assignments.size(); ++process_id) {
        //adding processes to machines
        get_machine(assignments[process_id])->add_process(process_id);
        //adding machine to processes
        Process *process = get_process(process_id);
        process->set_initial_machine_id(assignments[process_id]);
        process->set_current_machine_id(assignments[process_id]);
        process->set_assigned_status(true);

        //adding location to processes
        process->set_location_id(get_machine(assignments[process_id])->get_location_id());
        //adding neighborhood to processes
        process->set_neighborhood_id(get_machine(assignments[process_id])->get_neighborhood_id());

        //adding used machines, locations and neighborhoods to the service ownwer
        Service *service = get_service(process->get_service_id());

        if(!service->has_location(process->get_location_id())) {
            service->add_location(process->get_location_id());
        }

        if(!service->has_neighborhood(process->get_neighborhood_id())) {
            service->add_neighborhood(process->get_neighborhood_id());
        }

        if(!service->has_machine(process->get_current_machine_id())) {
            service->add_machine(process->get_current_machine_id());
        }

    }

}

void Instance::update_usage(unsigned int machine_id, unsigned int resource_id) {
    Machine *machine = get_machine(machine_id);
    uint32_t usage = 0;
    for(ProcessList::iterator it = machine->processes.begin(); it != machine->processes.end(); ++it) {
        usage += get_process(*it)->get_requirement(resource_id);
    }
    machine->set_usage(resource_id, usage);
}

void Instance::update_all_usages() {
    for(unsigned int i = 0; i < machines.size(); i++) {
        for (unsigned int j = 0; j < resources.size(); j++) {
            update_usage(i, j);
        }
    }

}


void Instance::init() {
    add_dependant_services();
    sort_processes_by_size();
    classify_services();
    sort_services_by_dependencies();
}

void Instance::add_dependant_services() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = services.begin(); service_iter != services.end(); ++service_iter) {
        ServiceList &dependencies = (*service_iter)->dependencies;
        for (unsigned int i = 0; i < dependencies.size(); ++i) {
            get_service(dependencies[i])->add_dependent((*service_iter)->get_id());
        }
    }
}

void Instance::sort_processes_by_size() {
    std::sort(sorted_processes.begin(), sorted_processes.end(), BiggerProcess(*this));

}

void Instance::classify_services() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = services.begin(); service_iter != services.end(); ++service_iter) {
        if((*service_iter)->get_spread_min() == 0 && (*service_iter)->dependencies.empty()) {
            less_restricted_services.push_back((*service_iter)->get_id());
            less_restricted_processes.insert(less_restricted_processes.end(), (*service_iter)->processes.begin(), (*service_iter)->processes.end());
        } else {
            restricted_services.push_back((*service_iter)->get_id());
        }

    }
}

void Instance::sort_services_by_dependencies() {
    std::sort(less_restricted_processes.begin(), less_restricted_processes.end(), BiggerProcess(*this));
    std::sort(restricted_services.begin(), restricted_services.end(), LessRestrictedService(*this));
}


void Instance::add_process(Process *process) {
    processes.push_back(process);
}

Process *Instance::get_process(unsigned int process_id) {
    return processes[process_id];
}


void Instance::add_machine(Machine *machine) {
    machines.push_back(machine);
}

Machine *Instance::get_machine(unsigned int machine_id) {
    return machines[machine_id];
}


void Instance::add_service(Service *service) {
    services.push_back(service);
}

Service *Instance::get_service(unsigned int service_id) {
    return services[service_id];
}


void Instance::add_resource(Resource *resource) {
    resources.push_back(resource);
}

Resource *Instance::get_resource(unsigned int resource_id) {
    return resources[resource_id];
}



void Instance::add_balance(Balance *balance) {
    balances.push_back(balance);
}

Balance *Instance::get_balance(unsigned int balance_id) {
    return balances[balance_id];
}


unsigned int Instance::get_weight_process_move_cost() {
    return weight_process_move_cost;
}

unsigned int Instance::get_weight_service_move_cost() {
    return weight_service_move_cost;
}

unsigned int Instance::get_weight_machine_move_cost() {
    return weight_machine_move_cost;
}


void Instance::print_services() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = services.begin(); service_iter != services.end(); ++service_iter) {
        (*service_iter)->print();
    }
}

void Instance::print_processes() {
    std::deque<Process*>::iterator process_iter;
    for (process_iter = processes.begin(); process_iter != processes.end(); ++process_iter) {
        (*process_iter)->print();
    }
}

void Instance::print_machines() {
    std::deque<Machine*>::iterator machine_iter;
    for (machine_iter = machines.begin(); machine_iter != machines.end(); ++machine_iter) {
        (*machine_iter)->print();
    }
}

void Instance::print() {

}

