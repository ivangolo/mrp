//
// Created by ivanedo on 12-06-15.
//

#include <iostream>
#include "Balance.h"

void Balance::set_id(unsigned int id) {
    this->id = id;
}

void Balance::set_resource(unsigned int resource_id, unsigned int num) {
    if(num == 1) {
        resource1_id = resource_id;
    } else {
        resource2_id = resource_id;
    }
}

void Balance::set_target(unsigned int target) {
    this->target = target;
}

void Balance::set_weight_balance_cost(unsigned int wbc) {
    weight_balance_cost = wbc;
}

unsigned int Balance::get_id() {
    return id;
}

unsigned int Balance::get_resource_id(unsigned int num) {
    if(num == 1) {
        return resource1_id;
    } else {
        return resource2_id;
    }
}

unsigned int Balance::get_target() {
    return target;
}

void Balance::print() {
    std::cout << "Recurso #" << id << ":<" << resource1_id << ", " << resource2_id << ", " << target << ">" << std::endl;
}