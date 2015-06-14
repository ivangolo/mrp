//
// Created by ivanedo on 13-06-15.
//

#include "Neighborhood.h"
#include <iostream>

using namespace std;

void Neighborhood::set_id(unsigned int id) {
    this->id = id;
}

unsigned int Neighborhood::get_id() {
    return id;
}
void Neighborhood::print() {
    cout << "Vecindario #" << id << endl;
}
