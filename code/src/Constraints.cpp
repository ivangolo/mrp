//
// Created by ivanedo on 23-06-15.
//

#include "Constraints.h"

//TODO implement constraints

bool Constraints::load_constraint(Instance &instance, unsigned int machine_id, unsigned int resource_id) {
    return false;
}

bool Constraints::transient_usage_constraint(Instance &instance, unsigned int machine_id,
                                                  unsigned int resource_id) {
    return false;
}

bool Constraints::spread_constraint(Instance &instance, unsigned int service_id) {
    return false;
}

bool Constraints::dependency_constraint(Instance &instance, unsigned int service_id) {
    return false;
}

bool Constraints::conflict_constraint(Instance &instance, unsigned int service_ids) {
    return false;
}
