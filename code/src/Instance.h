//
// Created by ivanedo on 13-06-15.
//

#ifndef MRP_INSTANCE_H
#define MRP_INSTANCE_H
#include <fstream>
#include <deque>
#include "Machine.h"
#include "Process.h"
#include "Service.h"
#include "Balance.h"
#include "Resource.h"

class Instance {
public:
    unsigned int weight_process_move_cost;
    unsigned int weight_service_move_cost;
    unsigned int weight_machine_move_cost;
    std::deque<Machine*> machines;
    std::deque<Process*> processes;
    std::deque<Service*> services;
    std::deque<Resource*> resources;
    std::deque<Balance*> balances;
    std::deque<Resource*>::iterator resource_iterator;
    std::deque<Machine*>::iterator machine_iterator;
    std::deque<Service*>::iterator service_iterator;
    std::deque<Process*>::iterator process_iterator;
    std::deque<Balance*>::iterator balance_iterator;
    Instance();
    ~Instance();
    void read_instance(std::ifstream &fin_instance);
    void print();

};


#endif //MRP_INSTANCE_H
