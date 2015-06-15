//
// Created by ivanedo on 12-06-15.
//

#include "Service.h"
#include <iostream>
#include <algorithm>
#include <iterator>

void Service::set_id(unsigned int id) {
    this->id = id;
}

void Service::set_spread_min(unsigned int sm) {
    spread_min = sm;
}

void Service::add_process(unsigned int process_id) {
    processes.push_back(process_id);
}
void Service::add_service(unsigned int service_id) {
    service_dependencies.push_back(service_id);
}
unsigned int Service::get_id() {
    return id;
}

void Service::print() {
    std::cout << "Servicio #" << std::endl;
    std::cout << "Procesos: [";
    copy(processes.begin(), processes.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]" << std::endl;
    std::cout << "Dependencias (servicios): [";
    copy(service_dependencies.begin(), service_dependencies.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
}