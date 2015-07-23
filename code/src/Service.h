//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SERVICE_H
#define MRP_SERVICE_H

#include "sets.h"

class Service {
    int id;
    unsigned int spread_min;
    unsigned int num_moved_processes;

public:
    ProcessList processes;
    MachineList machines;
    ServiceList dependencies;
    ServiceList dependents;
    LocationList locations;
    NeighborhoodList neighborhoods;
    Service();
    Service(int id, int spread_min);
    ~Service();

    void add_process(unsigned int process_id);
    void add_dependency(int service_id);
    void add_dependent(int service_id);
    void add_machine(int machine_id);
    void add_neighborhood(int neighborhood_id);
    void add_location(int location_id);
    void remove_machine(int machine_id);
    void remove_neighborhood(int neighborhood_id);
    void remove_location(int location_id);
    int get_id();
    unsigned int get_spread_min();
    void print();
    bool has_machine(int machine_id);
    bool has_neighborhood(int neighborhood_id);
    bool has_location(int location_id);
    unsigned int get_num_moved_processes();
    void increment_moved_processes();
    void decrement_moved_processes();
};


#endif //MRP_SERVICE_H
