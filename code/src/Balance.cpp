//
// Created by ivanedo on 12-06-15.
//

#include <iostream>
#include "Balance.h"

Balance::Balance() {
    std::cout << "Balance created" << std::endl;
}

Balance::Balance(unsigned int id, unsigned int target, unsigned int resource1_id, unsigned int resource2_id,
                 unsigned int weight_balance_cost) {
    this->id = id;
    this->target = target;
    this->resource1_id = resource1_id;
    this->resource2_id = resource2_id;
    this->weight_balance_cost = weight_balance_cost;
}

Balance::~Balance() {
    std::cout << "Balance " << id << " destroyed" << std::endl;
}

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
    return (num == 1) ? resource1_id : resource2_id;
}

unsigned int Balance::get_target() {
    return target;
}


unsigned int Balance::get_weight_balance_cost() {
    return weight_balance_cost;
}

void Balance::print() {
    std::cout << "Balance #" << id << ": <" << resource1_id << ", " << resource2_id << ", " << target << ">" << std::endl;
}