//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_PROCESS_H
#define MRP_PROCESS_H

#include "sets.h"

class Process {
    unsigned int id;
    unsigned int machine_id;
    unsigned int service_id;
    unsigned int process_move_cost;
    Requirements requirements;

public:
    void set_id(unsigned int id);
    void set_service_id(unsigned int service_id);
    void set_machine_id(unsigned int machine_id);
    void set_process_move_cost(unsigned int pmc);
    void add_resource_requirement(unsigned int requirement);
    unsigned int get_resource_requirement(unsigned int resource_id);

    unsigned int get_id();
    unsigned int get_service_id();
    unsigned int get_machine_id();
    unsigned int get_process_move_cost();
    void print();

};


#endif //MRP_PROCESS_H
