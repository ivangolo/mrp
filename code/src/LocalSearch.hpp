//
// Created by ivanedo on 03-08-15.
//

#ifndef MRP_LOCALSEARCH_H
#define MRP_LOCALSEARCH_H


#include "Solution.hpp"
#include "Instance.hpp"

class LocalSearch {

protected:
    Instance *instance;
    Solution *solution;
    double execution_time;
    unsigned int time_limit;
    SolutionNeighborhood neighborhood;
    void add_neighbour(unsigned int machine_id, int64_t cost);
    unsigned int get_min_machine();
    struct MachinesComparison {
        template <typename T>
        bool operator()(const T& left, const T& right) {
            return left.second < right.second;
        }
    };

public:
    LocalSearch(Instance *instance, Solution *solution);
    virtual ~LocalSearch();
    virtual void set_time_limit(unsigned int time_limit);
    virtual Solution * run() = 0;
    virtual void print() = 0;
    virtual double get_execution_time() = 0;
    
};


#endif //MRP_LOCALSEARCH_H
