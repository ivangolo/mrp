//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_PROCESS_H
#define MRP_PROCESS_H

#include "sets.h"

class Process {
    unsigned int id;
    unsigned int initial_machine_id;
    unsigned int current_machine_id;
    unsigned int service_id;
    unsigned int location_id;
    unsigned int neighborhood_id;
    unsigned int process_move_cost;
    Requirements requirements;

public:
    Process();
    Process(unsigned int id, unsigned int service_id);
    ~Process();
    void set_id(unsigned int id);
    void set_service_id(unsigned int service_id);
    void set_location_id(unsigned int location_id);
    void set_neighborhood_id(unsigned int neighborhood_id);
    void set_initial_machine_id(unsigned int initial_machine_id);
    void set_current_machine_id(unsigned int current_machine_id);
    void set_process_move_cost(unsigned int pmc);
    void add_requirement(unsigned int requirement);
    unsigned int get_requirement(unsigned int resource_id);

    unsigned int get_id();
    unsigned int get_service_id();
    unsigned int get_location_id();
    unsigned int get_neighborhood_id();
    unsigned int get_initial_machine_id();
    unsigned int get_current_machine_id();
    unsigned int get_process_move_cost();
    Requirements get_all_requirements();
    void print();

};


#endif //MRP_PROCESS_H
