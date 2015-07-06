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
    char type;
    std::deque<Machine*> machines;
    std::deque<Process*> processes;
    std::deque<Service*> services;
    std::deque<Resource*> resources;
    std::deque<Balance*> balances;
    ProcessList processes_sorted_by_req;
    Instance();
    ~Instance();
    void init(Assignments assignments);
    void compute_usage(unsigned int machine_id, unsigned int resource_id);
    void compute_all_usages();
    void read_instance_from_file(std::ifstream &fin_instance);
    void add_solution(std::deque<unsigned int> &original_solution);
    unsigned long int get_lower_bound();
    void compute_load_cost_lower_bound();
    void compute_balance_cost_lower_bound();
    void compute_lower_bound();
    void print_services();
    void add_dependant_services();
    void print();
    Machine* get_machine(unsigned int machine_id);
    Process* get_process(unsigned int process_id);
    Service* get_service(unsigned int service_id);
    Resource* get_resource(unsigned int resource_id);
    Balance* get_balance(unsigned int balance_id);
    unsigned int get_weight_process_move_cost();
    unsigned int get_weight_service_move_cost();
    unsigned int get_weight_machine_move_cost();
    void add_resource(Resource *resource);
    void add_machine(Machine *machine);
    void add_service(Service *service);
    void add_process(Process *process);
    void add_balance(Balance *balance);

};


#endif //MRP_INSTANCE_H
