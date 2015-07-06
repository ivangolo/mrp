//
// Created by ivanedo on 12-06-15.
//

#include "Machine.h"
#include <iostream>
#include <algorithm>
#include <iterator>


Machine::Machine() {
    std::cout << "Machine created";
}


Machine::Machine(unsigned int id, unsigned int neighborhood_id, unsigned int location_id) {
    this->id = id;
    this->neighborhood_id = neighborhood_id;
    this->location_id = location_id;
}

Machine::~Machine() {
    std::cout << "Machine " << id << " destroyed" << std::endl;
}

void Machine::add_process(unsigned int process_id) {
    processes.push_back(process_id);
}

void Machine::remove_process(unsigned int process_id) {
    std::deque<unsigned int>::iterator position = std::find(processes.begin(), processes.end(), process_id);
    if (position != processes.end()) {
        processes.erase(position);
    } // == vector.end() means the element was not found
}


void Machine::add_capacity(unsigned int capacity) {
    capacities.push_back(capacity);
}

void Machine::add_safety_capacity(unsigned int safety_capacity) {
    safety_capacities.push_back(safety_capacity);
}

void Machine::add_mmc(unsigned int mmc) {
    this->mmc.push_back(mmc);
}

unsigned int Machine::get_id() {
    return id;
}


unsigned int Machine::get_location_id() {
    return location_id;
}

unsigned int Machine::get_neighborhood_id() {
    return neighborhood_id;
}

void Machine::set_usage(unsigned int resource_id, unsigned long int usage) {
    usages[resource_id] = usage;
}


unsigned long int Machine::get_usage(unsigned int resource_id) {
    return usages[resource_id];
}

unsigned long int Machine::get_capacity(unsigned int resource_id) {
    return capacities[resource_id];
}


unsigned long  int Machine::get_scapacity(unsigned int resource_id) {
    return safety_capacities[resource_id];
}

void Machine::set_transient_usage(unsigned int resource_id, unsigned int usage) {
    transient_usages[resource_id] = usage;
}

unsigned long int Machine::get_transient_usage(unsigned int resource_id) {
    return transient_usages[resource_id];
}

void Machine::print() {
    std::cout << "Maquina #" << id << std::endl;
    std::cout << "Vecindario #" << neighborhood_id << std::endl;
    std::cout << "Localizacion #" << location_id << std::endl;
    std::cout << "Procesos: [";
    std::copy(processes.begin(), processes.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Usos: [";
    std::copy(usages.begin(), usages.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;

}

bool Machine::has_process(unsigned int process_id) {
    ProcessList::iterator position = std::find(processes.begin(), processes.end(), process_id);
    return (position != processes.end()); //true if process is found, false in the other case
}

unsigned int Machine::get_machine_move_cost(unsigned int machine_id) {
    return mmc[machine_id];
}
