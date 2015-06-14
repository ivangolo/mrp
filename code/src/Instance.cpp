//
// Created by ivanedo on 13-06-15.
//

#include "Instance.h"


unsigned int Instance::get_weight_process_move_cost() {
    return weight_process_move_cost;
}

unsigned int Instance::get_weight_service_move_cost() {
    return weight_service_move_cost;
}

unsigned int Instance::get_weight_machine_move_cost() {
    return weight_machine_move_cost;
}

void Instance::set_weight_process_move_cost(unsigned int weight) {
    weight_process_move_cost = weight;
}

void Instance::set_weight_service_move_cost(unsigned int weight) {
    weight_service_move_cost = weight;
}

void Instance::set_weight_machine_move_cost(unsigned int weight) {
    weight_machine_move_cost = weight;
}

void Instance::print() {

}
