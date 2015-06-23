//
// Created by ivanedo on 13-06-15.
//

#include "Location.h"

#include <iostream>


Location::Location() {
    std::cout << "Location created" << std::endl;
}

Location::Location(unsigned id) {
    this->id = id;
}

Location::~Location() {
    std::cout << "Location " << id << "destroyed" << std::endl;
}

void Location::set_id(unsigned int id) {
    this->id = id;
}
unsigned int Location::get_id() {
    return id;
}
void Location::print() {
    std::cout << "Localizacion #" << id << std::endl;
}