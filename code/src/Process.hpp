//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_PROCESS_H
#define MRP_PROCESS_H

#include "sets.hpp"

class Process {
private:
    unsigned int id;
    unsigned int initial_machine_id;
    unsigned int current_machine_id;
    unsigned int service_id;
    unsigned int location_id;
    unsigned int neighborhood_id;
    unsigned int process_move_cost;
    bool assigned;
    Requirements requirements;
    int64_t size;

public:
    Process(unsigned int id, unsigned int service_id);
    ~Process();
    void set_location_id(unsigned int location_id);
    void set_neighborhood_id(unsigned int neighborhood_id);
    void set_initial_machine_id(unsigned int initial_machine_id);
    void set_current_machine_id(unsigned int current_machine_id);
    void set_process_move_cost(unsigned int pmc);
    void add_requirement(int32_t requirement);
    int32_t get_requirement(unsigned int resource_id);
    unsigned int get_id();
    unsigned int get_service_id();
    unsigned int get_location_id();
    unsigned int get_neighborhood_id();
    unsigned int get_initial_machine_id();
    unsigned int get_current_machine_id();
    unsigned int get_process_move_cost();
    void print();
    int64_t get_size();
    void increment_process_size(int64_t increment);
    bool is_assigned();
    void set_assigned_status(bool assigned);

};


#endif //MRP_PROCESS_H
