//
// Created by ivanedo on 29-07-15.
//

#ifndef MRP_GREEDY_H
#define MRP_GREEDY_H


#include "Solution.h"

class Greedy {
    Instance *instance;
    Solution *solution;
    unsigned int assigned_processes;

public:
    Greedy();
    Greedy(Instance *instance, Solution *solution);
    ~Greedy();
    Solution * run();
    unsigned int get_num_assigned_processes();

};


#endif //MRP_GREEDY_H
