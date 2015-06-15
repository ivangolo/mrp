//
// Created by ivanedo on 12-06-15.
//

#include "Resource.h"
#include <iostream>

void Resource::set_id(unsigned int id) {
    this->id = id;
}

void Resource::set_weight_load_cost(unsigned int weight_load_cost) {
    this->weight_load_cost = weight_load_cost;
}

void Resource::set_transient(bool p) {
    transient = p;
}

bool Resource::is_transient() {
    return transient;
}

unsigned int Resource::get_id() {
    return id;
}

unsigned int Resource::get_weight_load_cost() {
    return weight_load_cost;
}

void Resource::print() {
    std::cout << "Recurso #" << id << std::endl;
}
