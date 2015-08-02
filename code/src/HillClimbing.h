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
    SolutionNeighborhood neighborhood;
public:

    enum Mode {
        INSTANCE_SEQUENCE,
        SORTED_BY_SIZE
    };
    Mode mode;

    HillClimbing();
    HillClimbing(Instance *instance, Solution *solution);
    ~HillClimbing();
    Solution * run();
    void set_time_limit(unsigned int time_limit);
    double get_execution_time();
    int32_t get_num_iterations();
    void print();
    void add_neighbour(unsigned int machine_id, int64_t cost_decrement);
    std::pair<unsigned int, int64_t> get_min_shift();

    void set_process_selection_mode(HillClimbing::Mode mode);

    struct ShiftMinCost {
        template <typename T>
        bool operator()(const T& left, const T& right) {
            return left.second < right.second;
        }
    };
};


#endif //MRP_HILLCLIMBING_H
