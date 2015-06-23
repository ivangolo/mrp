//
// Created by ivanedo on 12-06-15.
//

#include "Service.h"
#include <iostream>
#include <algorithm>
#include <iterator>


Service::Service() {
    std::cout << "Service created" << std::endl;
}

Service::Service(unsigned int id, unsigned int spread_min) {
    this->id = id;
    this->spread_min = spread_min;
}

Service::~Service() {
    std::cout << "Service " << id << " destroyed" << std::endl;
}

void Service::set_id(unsigned int id) {
    this->id = id;
}

void Service::set_spread_min(unsigned int sm) {
    spread_min = sm;
}

void Service::add_process(unsigned int process_id) {
    processes.push_back(process_id);
}
void Service::add_dependency(unsigned int service_id) {
    service_dependencies.push_back(service_id);
}
unsigned int Service::get_id() {
    return id;
}

void Service::print() {
    std::cout << "Servicio #" << std::endl;
    std::cout << "Dependencias (servicios): [";
    copy(service_dependencies.begin(), service_dependencies.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
}