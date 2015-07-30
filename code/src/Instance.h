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
    MachineList sorted_machines;
    ProcessList sorted_processes;
    ProcessList less_restricted_processes;
    ProcessList restricted_processes;
    ServiceList less_restricted_services;
    ServiceList restricted_services;
    Instance(std::ifstream &fin_instance);
    ~Instance();
    void init(Assignments assignments);
    void compute_usage(unsigned int machine_id, unsigned int resource_id);
    void compute_all_usages();
    void read_instance_from_file(std::ifstream &fin_instance);
    void add_solution(Assignments &original_solution);
    //int64_t get_lower_bound();
    //void compute_load_cost_lower_bound();
    //void compute_balance_cost_lower_bound();
    //void compute_lower_bound();
    void print_services();
    void print_processes();
    void print_machines();
    void add_dependant_services();
    void sort_processes_by_size();
    //void sort_machines_by_size();
    void sort_services_by_dependencies();
    void print();
    Process* get_process(unsigned int process_id);
    Machine* get_machine(unsigned int machine_id);
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
    unsigned long int get_num_of_processes();
    unsigned long int get_num_of_machines();
    unsigned long int get_num_of_services();
    void classify_services();

    struct BiggerMachine {
        Instance &instance;
        BiggerMachine(Instance &i) : instance(i) {}

        bool operator()(const unsigned int &left, const unsigned int &right) {
            return instance.get_machine(left)->get_size() > instance.get_machine(right)->get_size();
        }
    };

    struct BiggerProcess {
        Instance &instance;
        BiggerProcess(Instance &i) : instance(i) {}

        bool operator()(const unsigned int &left, const unsigned int &right) {
            return instance.get_process(left)->get_size() > instance.get_process(right)->get_size();
        }
    };

    struct LessRestrictedProcess {
        Instance &instance;  // Reference to parent
        LessRestrictedProcess(Instance &i) : instance(i) {}  // Initialise reference in constructor

        bool operator()(const unsigned int &left, const unsigned int &right) {
            return instance.get_service(left)->dependencies.size() < instance.get_service(right)->dependencies.size();
        }
    };

};


#endif //MRP_INSTANCE_H
