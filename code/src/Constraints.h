//
// Created by ivanedo on 23-06-15.
//

#ifndef MRP_CONSTRAINTS_H
#define MRP_CONSTRAINTS_H

#include "Instance.h"

class Constraints {
public:
    //load constraints -> for one machine and one resource
    static bool load_constraint(Instance &instance, unsigned int machine_id, unsigned int resource_id);
    //transient usage constraints -> for one machine and one resource
    static bool transient_usage_constraint(Instance &instance, unsigned int machine_id, unsigned int resource_id);
    //spread constraints -> for one service
    static bool spread_constraint(Instance &instance, unsigned int service_id);
    //dependency constraints -> for one service
    static bool dependency_constraint(Instance &instance, unsigned int service_id);
    //conflict constraints -> for one service
    static bool conflict_constraint(Instance &instance, unsigned int service_ids);
};


#endif //MRP_CONSTRAINTS_H
