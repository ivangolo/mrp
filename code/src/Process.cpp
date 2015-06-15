//
// Created by ivanedo on 12-06-15.
//

#include "Process.h"
#include <iostream>

void Process::set_id(unsigned int id) {
    this->id = id;
}

void Process::set_machine_id(unsigned int machine_id) {
    this->machine_id = machine_id;
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

unsigned int Process::get_machine_id() {
    return machine_id;
}

unsigned int Process::get_resource_requirement(unsigned int resource_id) {
    return requirements.at(resource_id);
}

unsigned int Process::get_process_move_cost() {
    return process_move_cost;
}

void Process::print() {
    std::cout << "Proceso #" << id << std::endl;
}