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
    unsigned int weight_process_move_cost;
    unsigned int weight_service_move_cost;
    unsigned int weight_machine_move_cost;
    //int64_t load_cost_lower_bound;
    //int64_t balance_cost_lower_bound;
    //char type;
public:
    std::deque<Machine*> machines;
    std::deque<Process*> processes;
    std::deque<Service*> services;
    std::deque<Resource*> resources;
    std::deque<Balance*> balances;
    std::deque<std::pair<unsigned int, int64_t >> sorted_processes;
    Instance(std::ifstream &fin_instance);
    ~Instance();
    void init(Assignments assignments);
    void compute_usage(int machine_id, int resource_id);
    void compute_all_usages();
    void read_instance_from_file(std::ifstream &fin_instance);
    void add_solution(Assignments &original_solution);
    //int64_t get_lower_bound();
    //void compute_load_cost_lower_bound();
    //void compute_balance_cost_lower_bound();
    //void compute_lower_bound();
    //void print_services();
    //void print_processes();
    //void print_machines();
    void add_dependant_services();
    void sort_process_by_size();
    void print();
    Process* get_process(unsigned int process_id);
    Machine* get_machine(int machine_id);
    Service* get_service(int service_id);
    Resource* get_resource(int resource_id);
    Balance* get_balance(int balance_id);
    unsigned int get_weight_process_move_cost();
    unsigned int get_weight_service_move_cost();
    unsigned int get_weight_machine_move_cost();
    void add_resource(Resource *resource);
    void add_machine(Machine *machine);
    void add_service(Service *service);
    void add_process(Process *process);
    void add_balance(Balance *balance);
    unsigned long int get_num_of_processes();
    unsigned long int get_num_of_machines();
//    unsigned long int get_num_of_resources();
//    unsigned long int get_num_of_balances();
//    unsigned long int get_num_of_services();
    //char get_type();
    //void set_type(char type);

    struct BiggerProcess {
        bool operator()(const std::pair<unsigned int, int64_t> &left, const std::pair<unsigned int, int64_t> &right) {
            return left.second > right.second;
        }
    };

};


#endif //MRP_INSTANCE_H
