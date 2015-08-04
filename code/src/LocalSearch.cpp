//
// Created by ivanedo on 03-08-15.
//

#include "LocalSearch.hpp"
#include <algorithm>

LocalSearch::LocalSearch(Instance *instance, Solution *solution): instance(instance), solution(solution), execution_time(0) {}

LocalSearch::~LocalSearch() {}

void LocalSearch::set_time_limit(unsigned int time_limit) {
    this->time_limit = time_limit;
}

double LocalSearch::get_execution_time() {
    return execution_time;
}

void LocalSearch::add_neighbour(unsigned int machine_id, int64_t cost) {
    neighborhood[machine_id] = cost;
}

unsigned int LocalSearch::get_min_machine() {
    return (*min_element(neighborhood.begin(), neighborhood.end(), MachinesComparison())).first;
}
