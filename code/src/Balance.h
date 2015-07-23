//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_BALANCE_H
#define MRP_BALANCE_H


class Balance {
    int id;
    int target;
    int resource1_id;
    int resource2_id;
    int weight_balance_cost;

public:

    Balance();
    Balance(int id, int target, int resource1_id, int resource2_id, int weight_balance_cost);
    ~Balance();

    int get_id();
    int get_target();
    int get_resource_id(int num);
    int get_weight_balance_cost();

    void print();

};


#endif //MRP_BALANCE_H
