//
// Created by ivanedo on 13-06-15.
//

#include "Instance.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>

Instance::Instance() {
    std::cout << "Instance created" << std::endl;
}

Instance::~Instance() {

    std::cout << "Eliminando recursos" << std::endl;
    std::for_each(resources.begin(), resources.end(), std::default_delete<Resource>());

    std::cout << "Eliminando maquinas" << std::endl;
    std::for_each(machines.begin(), machines.end(), std::default_delete<Machine>());

    std::cout << "Eliminando servicios" << std::endl;
    std::for_each(services.begin(), services.end(), std::default_delete<Service>());

    std::cout << "Eliminando procesos" << std::endl;
    std::for_each(processes.begin(), processes.end(), std::default_delete<Process>());

    std::cout << "Eliminando balances" << std::endl;
    std::for_each(balances.begin(), balances.end(), std::default_delete<Balance>());

    std::cout << "Instance destroyed" << std::endl;
}


void Instance::read_instance_from_file(std::ifstream &fin_instance) {

    //reading resources
    int num_resources;
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

        Machine *maquina = new Machine(i, neighborhood, location);

        //reading resource capacities
        for (int j = 0; j < num_resources; ++j) {
            fin_instance >> capacity;
            maquina->add_capacity(capacity);
        }
        //reading resource safety_capacities
        for (int k = 0; k < num_resources; ++k) {
            fin_instance >> safety_capacity;
            maquina->add_safety_capacity(safety_capacity);
        }
        //reading machine move costs
        for (int l = 0; l < num_machines; ++l) {
            fin_instance >> move_cost;
            maquina->add_machine_move_cost(move_cost);
        }

        add_machine(maquina);
    }

    //reading services
    unsigned int num_services;
    fin_instance >> num_services;
    for(unsigned int m = 0; m < num_services; ++m) {
        unsigned int spread_min, num_dependencies, service_id;
        fin_instance >> spread_min;
        fin_instance >> num_dependencies;

        Service *servicio = new Service(m, spread_min);

        if(num_dependencies > 0) {
            for(int i = 0; i < num_dependencies; ++i) {
                fin_instance >> service_id;
                servicio->add_dependency(service_id);
            }
            add_service(servicio);

        } else {//if the service hasn't dependencies
            add_service(servicio);
            continue;
        }
    }

    //reading processes
    unsigned int num_processes;
    fin_instance >> num_processes;
    for (unsigned int n = 0; n < num_processes; ++n) {
        unsigned int service_id, requirement, process_move_cost;
        fin_instance >> service_id;
        Process *proceso = new Process(n, service_id);

        //reading resource requirements
        for (int i = 0; i < num_resources; ++i) {
            fin_instance >> requirement;
            proceso->add_requirement(requirement);
        }
        // process move cost
        fin_instance >> process_move_cost;
        proceso->set_process_move_cost(process_move_cost);

        //adding the process to the service owner
        this->get_service(service_id)->add_process(n);

        add_process(proceso);
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


void Instance::add_solution(std::deque<unsigned int> &original_solution) {
    for(unsigned int i = 0; i < original_solution.size(); i++) {
        //adding processes to machines
        get_machine(original_solution[i])->add_process(i);
        //adding machine to processes
        Process *process = get_process(i);
        process->set_initial_machine_id(original_solution[i]);
        process->set_current_machine_id(original_solution[i]);

        //adding location to processes
        process->set_location_id(machines[original_solution[i]]->location_id);
        //adding neighborhood to processes
        process->set_neighborhood_id(machines[original_solution[i]]->neighborhood_id);

        //adding used machines, locations and neighborhoods to service ownwer
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

void Instance::compute_usage(unsigned int machine_id, unsigned int resource_id) {
    //initially, transient usages remains in 0
    Machine *machine = get_machine(machine_id);
    uint32_t usage = 0;
    for(std::deque<unsigned int>::iterator it = machine->processes.begin(); it != machine->processes.end(); ++it) {
        usage += get_process(*it)->get_requirement(resource_id);
    }
    machine->set_usage(resource_id, usage);
    //for a transient resource, its usage is initially zero
    machine->set_transient_usage(resource_id, 0);
}

void Instance::compute_all_usages() {
    for(unsigned int i = 0; i < machines.size(); i++) {
        for (unsigned int j = 0; j < resources.size(); j++) {
            compute_usage(i, j);
        }
    }

}

void Instance::compute_load_cost_lower_bound() {
    load_cost_lower_bound = 0;
    std::deque<Resource*>::iterator resource_iter;
    std::deque<Machine*>::iterator machine_iter;

    for (resource_iter = resources.begin(); resource_iter != resources.end(); ++resource_iter) {
        unsigned long int resource_usage = 0;
        unsigned long int resource_scapacity = 0;
        for (machine_iter = machines.begin(); machine_iter != machines.end(); ++machine_iter) {
            resource_usage += (*machine_iter)->get_usage((*resource_iter)->get_id());
            resource_scapacity += (*machine_iter)->get_scapacity((*resource_iter)->get_id());
        }
        load_cost_lower_bound += (resource_usage - resource_scapacity) * (*resource_iter)->get_weight_load_cost();
    }
}

void Instance::compute_balance_cost_lower_bound() {
    balance_cost_lower_bound = 0;
    std::deque<Balance*>::iterator balance_iter;
    std::deque<Machine*>::iterator machine_iter;

    for (balance_iter = balances.begin(); balance_iter != balances.end(); ++balance_iter) {
        unsigned long int a_r1 = 0;
        unsigned long int a_r2 = 0;
        unsigned int r1_id = (*balance_iter)->get_resource_id(1);
        unsigned int r2_id = (*balance_iter)->get_resource_id(2);

        for (machine_iter = machines.begin(); machine_iter != machines.end(); ++machine_iter) {
            //compute available capacity for resource 1 A(r1)
            a_r1 += ((*machine_iter)->get_capacity(r1_id) - (*machine_iter)->get_usage(r1_id));

            //compute available capacity for resource 2 A(r2)
            a_r2 += ((*machine_iter)->get_capacity(r2_id) - (*machine_iter)->get_usage(r2_id));

        }
        balance_cost_lower_bound += ((*balance_iter)->get_target() * a_r1 - a_r2) * (*balance_iter)->get_weight_balance_cost();
    }

}

void Instance::compute_lower_bound() {
    Instance::compute_load_cost_lower_bound();
    Instance::compute_balance_cost_lower_bound();
}

int64_t Instance::get_lower_bound() {
    return load_cost_lower_bound + balance_cost_lower_bound;
}

void Instance::init(Assignments assignments) {
    //do everything
    Instance::add_solution(assignments);
    Instance::add_dependant_services();
    Instance::compute_all_usages();
    Instance::compute_lower_bound();
}

void Instance::print_services() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = services.begin(); service_iter != services.end(); ++service_iter) {
        (*service_iter)->print();
    }
}

void Instance::add_dependant_services() {
    std::deque<Service*>::iterator service_iter;
    for (service_iter = services.begin(); service_iter != services.end(); ++service_iter) {
        ServiceList &dependencies = (*service_iter)->dependencies;
        for (int i = 0; i < dependencies.size(); ++i) {
            services[dependencies[i]]->add_dependent((*service_iter)->get_id());
        }
    }
}

Machine *Instance::get_machine(unsigned int machine_id) {
    return machines[machine_id];
}

Process *Instance::get_process(unsigned int process_id) {
    return processes[process_id];
}

Service *Instance::get_service(unsigned int service_id) {
    return services[service_id];
}

Resource *Instance::get_resource(unsigned int resource_id) {
    return resources[resource_id];
}

Balance *Instance::get_balance(unsigned int balance_id) {
    return balances[balance_id];
}

void Instance::print() {

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

void Instance::add_resource(Resource *resource) {
    resources.push_back(resource);
}

void Instance::add_machine(Machine *machine) {
    machines.push_back(machine);
}

void Instance::add_service(Service *service) {
    services.push_back(service);
}

void Instance::add_process(Process *process) {
    processes.push_back(process);
}

void Instance::add_balance(Balance *balance) {
    balances.push_back(balance);
}
