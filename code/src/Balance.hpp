//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_BALANCE_H
#define MRP_BALANCE_H


class Balance {
private:
    unsigned int id;
    unsigned int target;
    unsigned int r1_id;
    unsigned int r2_id;
    unsigned int weight_balance_cost;

public:
    Balance(unsigned int id, unsigned int target, unsigned int r1_id, unsigned int r2_id, unsigned int weight_balance_cost);
    ~Balance();

    unsigned int get_id();
    unsigned int get_target();
    unsigned int get_resource_id(unsigned int num);
    unsigned int get_weight_balance_cost();

    void print();

};


#endif //MRP_BALANCE_H
