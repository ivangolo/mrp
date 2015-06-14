//
// Created by ivanedo on 13-06-15.
//

#ifndef MRP_INSTANCE_H
#define MRP_INSTANCE_H


class Instance {
    unsigned int weight_process_move_cost;
    unsigned int weight_service_move_cost;
    unsigned int weight_machine_move_cost;

public:
    unsigned int get_weight_process_move_cost();
    unsigned int get_weight_service_move_cost();
    unsigned int get_weight_machine_move_cost();

    void set_weight_process_move_cost(unsigned int weight);
    void set_weight_service_move_cost(unsigned int weight);
    void set_weight_machine_move_cost(unsigned int weight);

    void print();

};


#endif //MRP_INSTANCE_H
