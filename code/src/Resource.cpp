//
// Created by ivanedo on 12-06-15.
//

#include "Resource.h"
#include <iostream>


Resource::Resource() {
    std::cout << "Resource created" << std::endl;
}

Resource::Resource(unsigned int id, unsigned int weight_load_cost, bool transient) {
    this->id = id;
    this->weight_load_cost = weight_load_cost;
    this->transient = transient;
}

Resource::~Resource() {
    //std::cout << "Resource " << id << " destroyed" << std::endl;
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
    std::cout << "Transitorio: " << std::boolalpha << transient << std::endl;
    std::cout << "Ponderación: " << weight_load_cost << std::endl;
}
