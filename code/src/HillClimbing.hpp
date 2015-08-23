//
// Created by ivanedo on 21-07-15.
//

#ifndef MRP_HILLCLIMBING_H
#define MRP_HILLCLIMBING_H

#include "Instance.hpp"
#include "Solution.hpp"
#include "LocalSearch.hpp"

class HillClimbing : LocalSearch {

private:
    int32_t num_iterations;

public:
    enum Mode {
        INSTANCE_SEQUENCE,
        SORTED_BY_SIZE
    };
    Mode mode;
    HillClimbing(Instance *instance, Solution *solution);
    ~HillClimbing();
    Solution * run();
    int32_t get_num_iterations();
    double get_execution_time();
    void print();
    void set_process_selection_mode(HillClimbing::Mode mode);
    void set_time_limit(unsigned int time_limit);


};


#endif //MRP_HILLCLIMBING_H
