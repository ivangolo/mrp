//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SERVICE_H
#define MRP_SERVICE_H

#include "sets.h"

class Service {
    unsigned int id;
    unsigned int spread_min;

public:
    ProcessList processes;
    MachineList machines;
    ServiceList dependencies;
    ServiceList dependents;
    LocationList locations;
    NeighborhoodList neighborhoods;
    Service();
    Service(unsigned int id, unsigned int spread_min);
    ~Service();

    void add_process(unsigned int process_id);
    void add_dependency(unsigned int service_id);
    void add_dependent(unsigned int service_id);
    void add_machine(unsigned int machine_id);
    void add_neighborhood(unsigned int neighborhood_id);
    void add_location(unsigned int location_id);
    void remove_machine(unsigned int machine_id);
    void remove_neighborhood(unsigned int neighborhood_id);
    void remove_location(unsigned int location_id);
    unsigned int get_id();
    unsigned int get_spread_min();
    void print();
};


#endif //MRP_SERVICE_H
