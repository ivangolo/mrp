//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SETS_H
#define MRP_SETS_H

#include <deque>

typedef std::deque<unsigned int> List;
typedef List Assignments;
typedef List MachineList;
typedef List ProcessList;
typedef List ServiceList;
typedef List LocationList;
typedef List NeighborhoodList;
typedef std::deque<unsigned long int> Capacities;
typedef Capacities Requirements;
typedef Capacities Usages;
typedef List MachineMoveCosts;

#endif //MRP_SETS_H
