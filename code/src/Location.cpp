//
// Created by ivanedo on 13-06-15.
//

#include "Location.h"

#include <iostream>

using namespace std;

void Location::set_id(unsigned int id) {
    this->id = id;
}
unsigned int Location::get_id() {
    return id;
}
void Location::print() {
    cout << "Localizacion #" << id << endl;
}