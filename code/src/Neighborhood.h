//
// Created by ivanedo on 13-06-15.
//

#ifndef MRP_NEIGHBORHOOD_H
#define MRP_NEIGHBORHOOD_H


class Neighborhood {
    unsigned int id;

public:
    Neighborhood();
    Neighborhood(unsigned int id);
    ~Neighborhood();
    void set_id(unsigned int id);
    unsigned int get_id();
    void print();

};


#endif //MRP_NEIGHBORHOOD_H
