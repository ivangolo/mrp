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
    SolutionNeighborhood neighborhood;

public:
    Greedy();
    Greedy(Instance *instance, Solution *solution);
    ~Greedy();
    Solution * run();
    unsigned int get_num_assigned_processes();
    std::pair<unsigned int, int64_t> get_min_assign();

    struct AssignMinCost {
        template <typename T>
        bool operator()(const T& left, const T& right) {
            return left.second < right.second;
        }
    };

};


#endif //MRP_GREEDY_H
