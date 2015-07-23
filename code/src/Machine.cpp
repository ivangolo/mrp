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


Machine::Machine(int id, int neighborhood_id, int location_id) {
    this->id = id;
    this->neighborhood_id = neighborhood_id;
    this->location_id = location_id;
}

Machine::~Machine() {
    //std::cout << "Machine " << id << " destroyed" << std::endl;
}

void Machine::add_process(unsigned int process_id) {
    processes.push_back(process_id);
}

void Machine::remove_process(unsigned int process_id) {
    ProcessList::iterator position = std::find(processes.begin(), processes.end(), process_id);
    if (position != processes.end()) {
        processes.erase(position);
    } // == vector.end() means the element was not found
}


void Machine::add_capacity(int32_t capacity) {
    capacities.push_back(capacity);
}

void Machine::add_safety_capacity(int32_t safety_capacity) {
    safety_capacities.push_back(safety_capacity);
}

void Machine::add_machine_move_cost(unsigned int mmc) {
    this->mmc.push_back(mmc);
}

int Machine::get_id() {
    return id;
}


int Machine::get_location_id() {
    return location_id;
}

int Machine::get_neighborhood_id() {
    return neighborhood_id;
}

void Machine::set_usage(int resource_id, int32_t usage) {
    usages[resource_id] = usage;
}


int32_t Machine::get_usage(int resource_id) {
    return usages[resource_id];
}

int32_t Machine::get_capacity(int resource_id) {
    return capacities[resource_id];
}

int32_t Machine::get_scapacity(int resource_id) {
    return safety_capacities[resource_id];
}

void Machine::set_transient_usage(int resource_id, int32_t usage) {
    transient_usages[resource_id] = usage;
}

int32_t Machine::get_transient_usage(int resource_id) {
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

int Machine::get_machine_move_cost(int machine_id) {
    return mmc[machine_id];
}

void Machine::add_usage(int32_t usage) {
    usages.push_back(usage);
}
