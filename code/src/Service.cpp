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

Service::Service(int id, int spread_min) {
    this->id = id;
    this->spread_min = spread_min;
    this->num_moved_processes = 0;
}

Service::~Service() {
    //std::cout << "Service " << id << " destroyed" << std::endl;
}

void Service::add_process(unsigned int process_id) {
    processes.push_back(process_id);
}

void Service::add_dependency(int service_id) {
    dependencies.push_back(service_id);
}

void Service::add_dependent(int service_id) {
    dependents.push_back(service_id);
}

int Service::get_id() {
    return id;
}

unsigned int Service::get_spread_min() {
    return spread_min;
}


void Service::add_machine(int machine_id) {
    machines.push_back(machine_id);
}

void Service::add_neighborhood(int neighborhood_id) {
    neighborhoods.push_back(neighborhood_id);
}

void Service::add_location(int location_id) {
    locations.push_back(location_id);
}

void Service::remove_machine(int machine_id) {
    std::deque<int>::iterator position = std::find(machines.begin(), machines.end(), machine_id);
    if (position != machines.end()) {
        machines.erase(position);
    } // == vector.end() means the element was not found
}

void Service::remove_neighborhood(int neighborhood_id) {
    std::deque<int>::iterator position = std::find(neighborhoods.begin(), neighborhoods.end(), neighborhood_id);
    if (position != neighborhoods.end()) {
        neighborhoods.erase(position);
    } // == vector.end() means the element was not found
}

void Service::remove_location(int location_id) {
    std::deque<int>::iterator position = std::find(locations.begin(), locations.end(), location_id);
    if (position != locations.end()) {
        locations.erase(position);
    } // == vector.end() means the element was not found
}

void Service::print() {
    std::cout << "Servicio #" << id << std::endl;
    std::cout << "Dependencias (servicios): [";
    copy(dependencies.begin(), dependencies.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Dependientes (servicios): [";
    copy(dependents.begin(), dependents.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Procesos: [";
    copy(processes.begin(), processes.end(), std::ostream_iterator<unsigned int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Localizaciones: [";
    copy(locations.begin(), locations.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
    std::cout << "Vecindarios: [";
    copy(neighborhoods.begin(), neighborhoods.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "]"<< std::endl;
}

bool Service::has_machine(int machine_id) {
    MachineList::iterator position = std::find(machines.begin(), machines.end(), machine_id);
    return (position != machines.end()); //true if process is found, false in the other case
}

bool Service::has_neighborhood(int neighborhood_id) {
    NeighborhoodList::iterator position = std::find(neighborhoods.begin(), neighborhoods.end(), neighborhood_id);
    return (position != neighborhoods.end()); //true if process is found, false in the other case
}

bool Service::has_location(int location_id) {
    LocationList::iterator position = std::find(locations.begin(), locations.end(), location_id);
    return (position != locations.end()); //true if process is found, false in the other case
}

unsigned int Service::get_num_moved_processes() {
    return num_moved_processes;
}

void Service::increment_moved_processes() {
    num_moved_processes++;
}

void Service::decrement_moved_processes() {
    if(num_moved_processes) {
        num_moved_processes--;
    }
}
