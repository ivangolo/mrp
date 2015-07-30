//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_MACHINE_H
#define MRP_MACHINE_H

#include <stdint.h>
#include "sets.h"

class Machine {
    unsigned int id;
    unsigned int neighborhood_id;
    unsigned int location_id;
    int64_t size;

public:
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
    void add_capacity(int32_t capacity);
    void add_safety_capacity(int32_t safety_capacity);
    void add_machine_move_cost(unsigned int mmc);
    void add_usage(int32_t usage);
    void add_transient_usage(int32_t usage);
    unsigned int get_id();
    unsigned int get_location_id();
    unsigned int get_neighborhood_id();
    void set_usage(unsigned int resource_id, int32_t usage);
    void set_transient_usage(unsigned int resource_id, int32_t usage);
    int32_t get_transient_usage(unsigned int resource_id);
    int32_t get_usage(unsigned int resource_id);
    int32_t get_capacity(unsigned int resource_id);
    int32_t get_scapacity(unsigned int resource_id);
    unsigned int get_machine_move_cost(unsigned int machine_id);
    void print();
    bool has_process(unsigned int process_id);
    int64_t get_size();
    void increment_machine_size(int64_t increment);


    //calculate costs here

};


#endif //MRP_MACHINE_H
