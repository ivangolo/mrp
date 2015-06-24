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

void Machine::push_process(unsigned int process_id) {
    processes.push_back(process_id);
}

unsigned int Machine::pop_process(unsigned int process_id) {
    return 0;
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

unsigned int Machine::get_neighborhood_id() {
    return neighborhood_id;
}

unsigned int Machine::get_location_id() {
    return location_id;
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
