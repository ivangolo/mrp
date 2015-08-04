//
// Created by ivanedo on 13-06-15.
//

#ifndef MRP_INSTANCE_H
#define MRP_INSTANCE_H
#include <fstream>
#include <deque>
#include "Machine.hpp"
#include "Process.hpp"
#include "Service.hpp"
#include "Balance.hpp"
#include "Resource.hpp"



class Instance {
private:
    unsigned int weight_process_move_cost;
    unsigned int weight_service_move_cost;
    unsigned int weight_machine_move_cost;

public:
    std::deque<Process*> processes;
    std::deque<Machine*> machines;
    std::deque<Resource*> resources;
    std::deque<Service*> services;
    std::deque<Balance*> balances;
    ProcessList sorted_processes;
    ProcessList less_restricted_processes;
    ServiceList less_restricted_services;
    ServiceList restricted_services;

    Instance();
    ~Instance();
    void init();
    void update_usage(unsigned int machine_id, unsigned int resource_id);
    void update_all_usages();
    void read_instance_from_file(std::ifstream &fin_instance);
    void add_assignments(Assignments assignments);
    void print_services();
    void print_processes();
    void print_machines();
    void add_dependant_services();
    void sort_processes_by_size();
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
    void classify_services();

    struct BiggerProcess {
        Instance &instance;
        BiggerProcess(Instance &i) : instance(i) {}

        bool operator()(const unsigned int &left, const unsigned int &right) {
            return instance.get_process(left)->get_size() > instance.get_process(right)->get_size();
        }
    };

    struct LessRestrictedService {
        Instance &instance;
        LessRestrictedService(Instance &i) : instance(i) {}

        bool operator()(const unsigned int &left, const unsigned int &right) {
            return instance.get_service(left)->dependencies.size() < instance.get_service(right)->dependencies.size();
        }
    };

};

#endif  //MRP_INSTANCE_H
