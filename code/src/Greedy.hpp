//
// Created by ivanedo on 29-07-15.
//

#ifndef MRP_GREEDY_H
#define MRP_GREEDY_H


#include "Solution.hpp"
#include "Instance.hpp"
#include "LocalSearch.hpp"

class Greedy : LocalSearch{

private:
    unsigned int assigned_processes;

public:
    Greedy(Instance *instance, Solution *solution);
    ~Greedy();
    Solution * run();
    unsigned int get_num_assigned_processes();
    void print();
    void set_time_limit(unsigned int time_limit);

};


#endif //MRP_GREEDY_H
