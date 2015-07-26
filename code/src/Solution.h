//
// Created by ivanedo on 26-06-15.
//

#ifndef MRP_SOLUTION_H
#define MRP_SOLUTION_H

#include "Instance.h"

class Solution {

    Instance *instance;
    Assignments assignments;
    int64_t load_cost;
    int64_t balance_cost;
    int64_t process_move_cost;
    int64_t service_move_cost;
    int64_t machine_move_cost;
    unsigned int max_num_of_moved_processes;

public:
    Solution();
    Solution(Instance *instance);
    Solution(Instance *instance, std::ifstream &in);
    ~Solution();

    int64_t get_load_cost();
    int64_t get_balance_cost();
    int64_t get_process_move_cost();
    int64_t get_service_move_cost();
    int64_t get_machine_move_cost();
    int64_t get_solution_cost();
    void read_solution_from_file(std::ifstream &in);
    void write_solution_to_file(std::ofstream &out);
    Assignments get_assignments();
    unsigned int get_current_assignment(unsigned int process_id);
    void print();
//    void print_assignments();
    void set_assignments(Assignments assignments);

    int64_t calc_load_cost(int machine_id, int resource_id);  //for one machine and over one resource
    int64_t calc_balance_cost(int machine_id, int balance_id);  //for one machine and over one balance
    int64_t calc_process_move_cost(unsigned int process_id);  //for one process
    unsigned int calc_service_move_cost(int service_id);  //for one service
    int64_t calc_machine_move_cost(unsigned int process_id);  //for one process


    int64_t calc_machine_load_cost(int machine_id);  //for one machine over all resources
    int64_t calc_machine_balance_cost(int machine_id);  //for one machine over all balances


    int64_t calc_total_load_cost();  //calc_machine_load_cost for all machines
    int64_t calc_total_balance_cost();  //calc_machine_balance_cost for all machines
    int64_t calc_total_service_move_cost();  //calc_service_move_cost for all services
    int64_t calc_total_process_move_cost();  //calc_process_move_cost for all process
    int64_t calc_total_machine_move_cost();  //calc_machine_move_cost for all process

    int64_t get_load_cost_with_process(unsigned int process_id, int machine_id);
    int64_t get_load_cost_without_process(unsigned int process_id, int machine_id);

    int64_t get_balance_cost_with_process(unsigned int process_id, int machine_id);
    int64_t get_balance_cost_without_process(unsigned int process_id, int machine_id);

    void update_solution_load_cost();
    void update_solution_balance_cost();
    void update_solution_process_move_cost();
    void update_solution_service_move_cost();
    void update_solution_machine_move_cost();
    void update_solution_costs();


    int64_t calc_delta_load_cost_with_shift(unsigned int process_id, int machine_id);
    int64_t calc_delta_balance_cost_with_shift(unsigned int process_id, int machine_id);
    int64_t calc_delta_process_move_cost_with_shift(unsigned int process_id, int machine_id);
    int64_t calc_delta_service_move_cost_with_shift(unsigned int process_id, int machine_id);
    int64_t calc_delta_machine_move_cost_with_shift(unsigned int process_id, int machine_id);
    int64_t calc_delta_cost_with_shift(unsigned int process_id, int machine_id);


    bool check_capacity_with_shift(unsigned int process_id, int machine_id);
    bool check_conflict_with_shift(unsigned int process_id, int machine_id);
    bool check_spread_with_shift(unsigned int process_id, int machine_id);
    bool check_dependency_with_shift(unsigned int process_id, int machine_id);
    bool check_transient_usage_with_shift(unsigned int process_id, int machine_id);
    bool check_shift(unsigned int process_id, int machine_id);

    //Reasignments
    void shift_process(unsigned int process_id, unsigned int machine_id);

    //Assignments
    void assign_process(unsigned int process_id, unsigned int machine_id);

    //checkers
    bool check_assignment(unsigned int process_id, unsigned int machine_id);
    bool check_capacity_with_assignment(unsigned int process_id, int machine_id);
    bool check_conflict_with_assignment(unsigned int process_id, int machine_id);
    bool check_spread_with_assignment(unsigned int process_id, int machine_id);
    bool check_dependency_with_assignment(unsigned int process_id, int machine_id);

};


#endif //MRP_SOLUTION_H
