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
    this->size = 0;
    this->initial_machine_id = 60000;
    this->current_machine_id = 60000;
    this->neighborhood_id = 60000;
    this->location_id = 60000;
    this->assigned = false;

}

Process::~Process() {
   // std::cout << "Process " << id << " destroyed" << std::endl;
}

void Process::set_current_machine_id(unsigned int current_machine_id) {
    this->current_machine_id = current_machine_id;
}

void Process::set_initial_machine_id(unsigned int initial_machine_id) {
    this->initial_machine_id = initial_machine_id;
}

void Process::set_process_move_cost(unsigned int pmc) {
    process_move_cost = pmc;
}

void Process::add_requirement(int32_t requirement) {
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

int32_t Process::get_requirement(unsigned int resource_id) {
    return requirements[resource_id];
}

unsigned int Process::get_process_move_cost() {
    return process_move_cost;
}


void Process::set_location_id(unsigned int location_id) {
    this->location_id = location_id;
}

void Process::set_neighborhood_id(unsigned int neighborhood_id) {
    this->neighborhood_id = neighborhood_id;
}

unsigned int Process::get_location_id() {
    return location_id;
}

unsigned int Process::get_neighborhood_id() {
    return neighborhood_id;
}

void Process::print() {
    std::cout << "Proceso # " << id << std::endl;
    std::cout << "Servicio # " << service_id << std::endl;
    std::cout << "Process move cost: " << process_move_cost << std::endl;
    std::cout << "Requerimientos: [";
    copy(requirements.begin(), requirements.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Tamaño: " << size << std::endl;
    std::cout << "Máquina: " << get_current_machine_id() << std::endl;
    std::cout << "Vecindario: " << get_neighborhood_id() << std::endl;
    std::cout << "Localización: " << get_location_id() << std::endl;
    std::cout << "___________________________________" << std::endl;
}

int64_t Process::get_size() {
    return size;
}


void Process::increment_process_size(int64_t increment) {
    size += increment;
}


bool Process::is_assigned() {
    return assigned;
}


void Process::set_assigned_status(bool assigned) {
    this->assigned = assigned;
}
