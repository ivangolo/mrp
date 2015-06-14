//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_BALANCE_H
#define MRP_BALANCE_H


class Balance {
    unsigned int id;
    unsigned int target;
    unsigned int resource1_id;
    unsigned int resource2_id;
    unsigned int weight_balance_cost;

public:

    void set_id(unsigned int id);
    void set_target(unsigned int target);
    void set_resource(unsigned int resource_id, unsigned int num);
    void set_weight_balance_cost(unsigned int wbc);

    unsigned int get_id();
    unsigned int get_target();
    unsigned int get_resource_id(unsigned int num);
    void print();

};


#endif //MRP_BALANCE_H
