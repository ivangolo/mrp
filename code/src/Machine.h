//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_MACHINE_H
#define MRP_MACHINE_H

#include "sets.h"


class Machine {
    unsigned int id;
    unsigned int neighborhood_id;
    unsigned int location_id;
    Capacities capacities;
    Capacities safety_capacities;
    ProcessList processes;
    MachineMoveCosts mmc;

public:
    void set_id(unsigned int id);
    void set_neighborhood_id(unsigned int neighborhood_id);
    void set_location_id(unsigned int location_id);
    void add_capacity(unsigned int capacity);
    void add_safety_capacity(unsigned int capacity);
    void add_machine_move_cost(unsigned int cost);
    void push_process(unsigned int process_id);
    unsigned int pop_process(unsigned int process_id);

    unsigned int get_id();
    unsigned int get_neighborhood_id();
    unsigned int get_location_id();
    void print();
};


#endif //MRP_MACHINE_H
