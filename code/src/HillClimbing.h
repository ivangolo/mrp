//
// Created by ivanedo on 21-07-15.
//

#ifndef MRP_HILLCLIMBING_H
#define MRP_HILLCLIMBING_H

#include "Instance.h"
#include "Solution.h"

class HillClimbing {
    Instance *instance;
    Solution *solution;
    double execution_time;
    int32_t num_iterations;
    unsigned int time_limit;

public:
    HillClimbing();
    HillClimbing(Instance *instance, Solution *solution);
    ~HillClimbing();
    Solution * run(bool sorted = false);
    void set_time_limit(unsigned int time_limit);
    double get_execution_time();
    int32_t get_num_iterations();
    void print();
};


#endif //MRP_HILLCLIMBING_H
