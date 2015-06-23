//
// Created by ivanedo on 12-06-15.
//

#include "Process.h"
#include <iostream>
#include <algorithm>
#include <iterator>


Process::Process() {
    std::cout << "Process created";
}

Process::Process(unsigned int id, unsigned int service_id) {
    this->id = id;
    this->service_id = service_id;
}

Process::~Process() {
    std::cout << "Process " << id << " destroyed" << std::endl;
}

void Process::set_id(unsigned int id) {
    this->id = id;
}

void Process::set_current_machine_id(unsigned int current_machine_id) {
    this->current_machine_id = current_machine_id;
}

void Process::set_initial_machine_id(unsigned int initial_machine_id) {
    this->initial_machine_id = initial_machine_id;
}


void Process::set_service_id(unsigned int service_id) {
    this->service_id = service_id;
}

void Process::set_process_move_cost(unsigned int pmc) {
    process_move_cost = pmc;
}

void Process::add_resource_requirement(unsigned int requirement) {
    requirements.push_back(requirement);
}

unsigned int Process::get_id() {
    return id;
}

unsigned int Process::get_service_id() {
    return service_id;
}

unsigned int Process::get_initial_machine_id() {
    return initial_machine_id;
}

unsigned int Process::get_current_machine_id() {
    return current_machine_id;
}

unsigned int Process::get_resource_requirement(unsigned int resource_id) {
    return requirements.at(resource_id);
}

unsigned int Process::get_process_move_cost() {
    return process_move_cost;
}

void Process::print() {
    std::cout << "Proceso # " << id << std::endl;
    std::cout << "Servicio # " << service_id << std::endl;
    std::cout << "Process move cost: " << process_move_cost << std::endl;
    std::cout << "Requerimientos: [";
    copy(requirements.begin(), requirements.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
}