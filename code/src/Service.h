//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_SERVICE_H
#define MRP_SERVICE_H

#include "sets.h"

class Service {
    unsigned int id;
    unsigned int spread_min;
    ProcessList processes;
    ServiceList service_dependencies;

public:
    void set_id(unsigned int id);
    void set_spread_min(unsigned int sm);
    void add_process(unsigned int process_id);
    void add_service(unsigned int service_id);
    unsigned int get_id();
    void print();
};


#endif //MRP_SERVICE_H
