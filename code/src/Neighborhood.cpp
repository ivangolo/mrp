//
// Created by ivanedo on 13-06-15.
//

#include "Neighborhood.h"
#include <iostream>


Neighborhood::Neighborhood() {
    std::cout << "Neighborhood created" << std::endl;
}

Neighborhood::Neighborhood(unsigned int id) {
    this->id = id;

}

Neighborhood::~Neighborhood() {
    std::cout << "Neighborhood " << id << "destroyed" << std::endl;
}

void Neighborhood::set_id(unsigned int id) {
    this->id = id;
}

unsigned int Neighborhood::get_id() {
    return id;
}
void Neighborhood::print() {
    std::cout << "Vecindario #" << id << std::endl;
}
