//
// Created by ivanedo on 26-06-15.
//

#ifndef MRP_SOLUTION_H
#define MRP_SOLUTION_H

#include "Instance.h"

class Solution {

    Instance *instance;
    Assignments assignments;
    unsigned long int load_cost;
    unsigned long int balance_cost;
    unsigned long int process_move_cost;
    unsigned long int service_move_cost;
    unsigned long int machine_move_cost;

public:
    Solution();
    Solution(Instance *instance);
    ~Solution();

    unsigned long int get_load_cost();
    unsigned long int get_balance_cost();
    unsigned long int get_process_move_cost();
    unsigned long int get_service_move_cost();
    unsigned long int get_machine_move_cost();
    unsigned long int get_solution_cost();
    void read_solution_from_file(std::ifstream &in);
    void write_solution_to_file(std::ofstream &out);
    Assignments get_assignments();
    unsigned int get_current_assignment(unsigned int process_id);
    void print();


    unsigned long int calc_load_cost(unsigned int machine_id, unsigned int resource_id);  //for one machine and over one resource
    unsigned long int calc_balance_cost(unsigned int machine_id, unsigned int balance_id);  //for one machine and over one balance
    unsigned long int calc_process_move_cost(unsigned int process_id);  //for one process
    unsigned long int calc_service_move_cost(unsigned int service_id);  //for one service
    unsigned long int calc_machine_move_cost(unsigned int process_id);  //for one process


    unsigned long int calc_machine_load_cost(unsigned int machine_id);  //for one machine over all resources
    unsigned long int calc_machine_balance_cost(unsigned machine_id);  //for one machine over all balances


    unsigned long int calc_total_load_cost();  //calc_machine_load_cost for all machines
    unsigned long int calc_total_balance_cost();  //calc_machine_balance_cost for all machines
    unsigned long int calc_total_service_move_cost();  //calc_service_move_cost for all services
    unsigned long int calc_total_process_move_cost();  //calc_process_move_cost for all process
    unsigned long int calc_total_machine_move_cost();  //calc_machine_move_cost for all process


    void update_solution_load_cost();
    void update_solution_balance_cost();
    void update_solution_process_move_cost();
    void update_solution_service_move_cost();
    void update_solution_machine_move_cost();
    void update_solution_costs();


    unsigned long int calc_load_cost_with_shift(unsigned int process_id, unsigned int machine_id);
    unsigned long int calc_balance_cost_with_shift(unsigned int process_id, unsigned int machine_id);
    unsigned long int calc_process_move_cost_with_shift(unsigned int process_id, unsigned int machine_id);
    unsigned long int calc_service_cost_with_shift(unsigned int process_id, unsigned int machine_id);
    unsigned long int calc_machine_cost_with_shift(unsigned int process_id, unsigned int machine_id);

    unsigned long int calc_cost_with_shift(unsigned int process_id, unsigned int machine_id);


    //Constraints
    bool check_capacity_constraint(unsigned int machine_id, unsigned int resource_id);
    bool check_conflict_constraint(unsigned int service_id);
    bool check_spread_constraint(unsigned int service_id);
    bool check_dependency_constraint(unsigned int service_id);

    bool check_all_capacity_constraints();
    bool check_all_conflict_constraints();
    bool check_all_spread_constraints();
    bool check_all_dependency_constraints();

    bool check_solution();

    bool check_capacity_with_shift(unsigned int process_id, unsigned int machine_id);
    bool check_conflict_with_shift(unsigned int process_id, unsigned int machine_id);
    bool check_spread_with_shift(unsigned int process_id, unsigned int machine_id);
    bool check_dependency_with_shift(unsigned int process_id, unsigned int machine_id);
    bool check_transient_usage_with_shift(unsigned int process_id, unsigned int machine_id);
    bool check_shift(unsigned int process_id, unsigned int machine_id);

    //Reasignments
    void shift_process(unsigned int process_id, unsigned int machine_id);
};


#endif //MRP_SOLUTION_H
