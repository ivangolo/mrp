//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_MACHINE_H
#define MRP_MACHINE_H

#include <stdint.h>
#include "sets.h"

class Machine {
    int id;
    int neighborhood_id;
    int location_id;

public:
    Capacities capacities;
    Capacities safety_capacities;
    ProcessList processes;
    MachineMoveCosts mmc;
    Usages usages;
    Usages transient_usages;

    Machine();
    Machine(int id, int neighborhood_id, int location_id);
    ~Machine();

    void add_process(unsigned int process_id);
    void remove_process(unsigned int process_id);
    void add_capacity(int32_t capacity);
    void add_safety_capacity(int32_t safety_capacity);
    void add_machine_move_cost(unsigned int mmc);
    void add_usage(int32_t usage);
    int get_id();
    int get_location_id();
    int get_neighborhood_id();
    void set_usage(int resource_id, int32_t usage);
    void set_transient_usage(int resource_id, int32_t usage);
    int32_t get_transient_usage(int resource_id);
    int32_t get_usage(int resource_id);
    int32_t get_capacity(int resource_id);
    int32_t get_scapacity(int resource_id);
    int get_machine_move_cost(int machine_id);
    void print();
    bool has_process(unsigned int process_id);


    //calculate costs here

};


#endif //MRP_MACHINE_H
