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
    unsigned long int load_cost_lower_bound;
    unsigned long int balance_cost_lower_bound;
    std::deque<Machine*> machines;
    std::deque<Process*> processes;
    std::deque<Service*> services;
    std::deque<Resource*> resources;
    std::deque<Balance*> balances;
    Instance();
    ~Instance();
    void init(std::ifstream &fin_instance, std::deque<unsigned int> &original_solution);
    void compute_usage(unsigned int machine_id, unsigned int resource_id);
    void compute_all_usages();
    void read_instance(std::ifstream &fin_instance);
    void add_solution(std::deque<unsigned int> &original_solution);
    unsigned long int get_lower_bound();
    void compute_load_cost_lower_bound();
    void compute_balance_cost_lower_bound();
    void compute_lower_bound();
    void print();

};


#endif //MRP_INSTANCE_H
