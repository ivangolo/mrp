//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_RESOURCE_H
#define MRP_RESOURCE_H


class Resource {
    unsigned int id;
    unsigned int weight_load_cost;
    bool transient;


public:
    void set_id(unsigned int  id);
    void set_weight_load_cost(unsigned int weight_load_cost);
    void set_transient(bool p);
    bool is_transient();
    unsigned int get_id();
    unsigned int get_weight_load_cost();
    void print();

};


#endif //MRP_RESOURCE_H
