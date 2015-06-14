//
// Created by ivanedo on 12-06-15.
//

#include "Machine.h"
#include <iostream>

void Machine::set_id(unsigned int id) {
    this->id = id;
}

void Machine::set_neighborhood_id(unsigned int neighborhood_id) {
    this->neighborhood_id = neighborhood_id;
}

void Machine::set_location_id(unsigned int location_id) {
    this->location_id = location_id;
}

void Machine::add_capacity(unsigned int capacity) {
    capacities.push_back(capacity);
}

void Machine::add_safety_capacity(unsigned int capacity) {
    safety_capacities.push_back(capacity);
}

void Machine::add_machine_move_cost(unsigned int cost) {
    mmc.push_back(cost);
}

void Machine::push_process(unsigned int process_id) {
    processes.push_back(process_id);
}

unsigned int Machine::pop_process(unsigned int process_id) {
    return 0;
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

}
