//
// Created by ivanedo on 12-06-15.
//

#include <iostream>
#include "Balance.hpp"

Balance::Balance(unsigned int id, unsigned int target, unsigned int r1_id, unsigned int r2_id, unsigned int weight_balance_cost)
        :id(id), target(target), r1_id(r1_id), r2_id(r2_id), weight_balance_cost(weight_balance_cost) {};

Balance::~Balance() {}

unsigned int Balance::get_id() {
    return id;
}

unsigned int Balance::get_resource_id(unsigned int num) {
    return (num == 1) ? r1_id : r2_id;
}

unsigned int Balance::get_target() {
    return target;
}


unsigned int Balance::get_weight_balance_cost() {
    return weight_balance_cost;
}

void Balance::print() {
    std::cout << "Balance #" << id << ": <" << r1_id << ", " << r2_id << ", " << target << ">" << std::endl;
}