//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SETS_H
#define MRP_SETS_H

#include <deque>
#include <cstdint>

typedef std::deque<unsigned int> List;
typedef List Assignments;
typedef List MachineList;
typedef List ProcessList;
typedef List ServiceList;
typedef List LocationList;
typedef List NeighborhoodList;
typedef List MachineMoveCosts;
typedef std::deque<int32_t> Capacities;
typedef Capacities Requirements;
typedef Capacities Usages;

#endif //MRP_SETS_H
