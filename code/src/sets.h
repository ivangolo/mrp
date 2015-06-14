//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SETS_H
#define MRP_SETS_H

#include <deque>

using namespace std;

typedef deque<unsigned int> ProcessList;
typedef deque<unsigned int> ServiceList;
typedef deque<unsigned int> Capacities;
typedef Capacities Requirements;
typedef deque<unsigned int> MachineMoveCosts;

#endif //MRP_SETS_H
