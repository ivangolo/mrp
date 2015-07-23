//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_PROCESS_H
#define MRP_PROCESS_H

#include "sets.h"

class Process {
    unsigned int id;
    int initial_machine_id;
    int current_machine_id;
    int service_id;
    int location_id;
    int neighborhood_id;
    int process_move_cost;
    Requirements requirements;
    int64_t size;

public:
    Process();
    Process(unsigned int id, int service_id);
    ~Process();
    void set_location_id(int location_id);
    void set_neighborhood_id(int neighborhood_id);
    void set_initial_machine_id(int initial_machine_id);
    void set_current_machine_id(int current_machine_id);
    void set_process_move_cost(int pmc);
    void add_requirement(int32_t requirement);
    int32_t get_requirement(int resource_id);
    unsigned int get_id();
    int get_service_id();
    int get_location_id();
    int get_neighborhood_id();
    int get_initial_machine_id();
    int get_current_machine_id();
    int get_process_move_cost();
    void print();
    int64_t get_size();
    void increment_process_size(int64_t increment);

};


#endif //MRP_PROCESS_H
