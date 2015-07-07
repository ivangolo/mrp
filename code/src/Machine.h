//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_MACHINE_H
#define MRP_MACHINE_H

#include <stdint.h>
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
    void add_capacity(uint32_t capacity);
    void add_safety_capacity(uint32_t safety_capacity);
    void add_mmc(unsigned int mmc);
    unsigned int get_id();
    unsigned int get_location_id();
    unsigned int get_neighborhood_id();
    void set_usage(unsigned int resource_id, uint32_t usage);
    void set_transient_usage(unsigned int resource_id, uint32_t usage);
    uint32_t get_transient_usage(unsigned int resource_id);
    uint32_t get_usage(unsigned int resource_id);
    uint32_t get_capacity(unsigned int resource_id);
    uint32_t get_scapacity(unsigned int resource_id);
    unsigned int get_machine_move_cost(unsigned int machine_id);
    void print();
    bool has_process(unsigned int process_id);


    //calculate costs here

};


#endif //MRP_MACHINE_H
