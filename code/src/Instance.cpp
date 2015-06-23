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


void Instance::read_instance(std::ifstream &fin_instance) {

    //reading resources
    unsigned int num_resources;
    fin_instance >> num_resources;
    for(unsigned int i = 0; i < num_resources; ++i) {
        unsigned short int transient;
        unsigned int weight;
        fin_instance >> transient;
        fin_instance >> weight;

        resources.push_back(new Resource(i, weight, (bool)transient));
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
            maquina->add_mmc(move_cost);
        }

        machines.push_back(maquina);
    }

    //reading services
    unsigned int num_services;
    fin_instance >> num_services;
    for(int m = 0; m < num_services; ++m) {
        unsigned int spread_min, num_dependencies, service_id;
        fin_instance >> spread_min;
        fin_instance >> num_dependencies;

        Service *servicio = new Service(m,spread_min);

        if(num_dependencies > 0) {
            for(int i = 0; i < num_dependencies; ++i) {
                fin_instance >> service_id;
                servicio->add_dependency(service_id);
            }
            services.push_back(servicio);

        } else {//si no depende de otros servicios
            services.push_back(servicio);
            continue;
        }
    }

    //reading processes
    unsigned int num_processes;
    fin_instance >> num_processes;
    for (int n = 0; n < num_processes; ++n) {
        unsigned int service_id, requirement, process_move_cost;
        fin_instance >> service_id;
        Process *proceso = new Process(n,service_id);

        //reading resource requirements
        for (int i = 0; i < num_resources; ++i) {
            fin_instance >> requirement;
            proceso->add_resource_requirement(requirement);
        }
        // process move cost
        fin_instance >> process_move_cost;
        proceso->set_process_move_cost(process_move_cost);

        processes.push_back(proceso);
    }

    //reading balances
    unsigned int num_balances;
    fin_instance >> num_balances;
    for (int i = 0; i < num_balances; ++i) {
        unsigned int resource1, resource2, target, weight;
        fin_instance >> resource1;
        fin_instance >> resource2;
        fin_instance >> target;
        fin_instance >> weight;

        balances.push_back(new Balance(i, target, resource1, resource2, weight));

    }

    //reading final weights
    fin_instance >> weight_process_move_cost;
    fin_instance >> weight_service_move_cost;
    fin_instance >> weight_machine_move_cost;

}

void Instance::print() {

}
