//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_MACHINE_H
#define MRP_MACHINE_H

#include "sets.h"

class Machine {
public:

    unsigned int id;
    unsigned int neighborhood_id;
    unsigned int location_id;
    Capacities capacities;
    Capacities safety_capacities;
    ProcessList processes;
    MachineMoveCosts mmc;
    Usages usages;
    Usages transient_usages;

    Machine();
    Machine(unsigned int id, unsigned int neighborhood_id, unsigned int location_id);
    ~Machine();

    void add_process(unsigned int process_id);
    void remove_process(unsigned int process_id);
    void add_capacity(unsigned int capacity);
    void add_safety_capacity(unsigned int safety_capacity);
    void add_mmc(unsigned int mmc);
    unsigned int get_id();
    unsigned int get_location_id();
    unsigned int get_neighborhood_id();
    void set_usage(unsigned int resource_id, unsigned int usage);
    void set_transient_usage(unsigned int resource_id, unsigned int usage);
    unsigned int get_transient_usage(unsigned int resource_id);
    unsigned int get_usage(unsigned int resource_id);
    unsigned int get_capacity(unsigned int resource_id);
    void print();


    //calculate costs here

};


#endif //MRP_MACHINE_H
