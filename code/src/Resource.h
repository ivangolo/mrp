//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_RESOURCE_H
#define MRP_RESOURCE_H


class Resource {
    int id;
    int weight_load_cost;
    bool transient;


public:
    Resource();
    Resource(int id, int weight_load_cost, bool transient);
    ~Resource();
    bool is_transient();
    int get_id();
    int get_weight_load_cost();
    void print();

};


#endif //MRP_RESOURCE_H
