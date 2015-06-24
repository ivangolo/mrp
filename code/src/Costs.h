//
// Created by ivanedo on 23-06-15.
//

#ifndef MRP_COSTS_H
#define MRP_COSTS_H

#include "Instance.h"

class Costs {
public:
    static unsigned int load_cost(Instance &instance, unsigned int machine_id, unsigned int resource_id); //for one machine and one resource
    static unsigned int balance_cost(Instance &instance, unsigned int machine_id, unsigned int balance_id); //for one machine and one resource
    static unsigned int process_move_cost(Instance &instance, unsigned int process_id); //for one process
    static unsigned int service_move_cost(Instance &instance, unsigned int service_id); //for one service
    static unsigned int machine_move_cost(Instance &instance, unsigned int process_id); //for one process

    static unsigned int machine_load_cost(Instance &instance, unsigned int machine_id);
    static unsigned int machine_balance_cost(Instance &instance, unsigned machine_id);
    static unsigned int machine_cost(Instance &instance, unsigned int machine_id);
    static unsigned int total_machine_cost(Instance &instance);

    static unsigned int process_cost(Instance &instance, unsigned int process_id);
    static unsigned int total_process_cost(Instance &instance);

    static unsigned int total_service_cost(Instance &instance);

    static unsigned int total_cost(Instance &instance);
};



#endif //MRP_COSTS_H
