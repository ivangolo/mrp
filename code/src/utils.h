//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_LIBUTIL_H
#define MRP_LIBUTIL_H

#include <deque>

void show_help_info(char *s);
void get_original_solution(std::ifstream &in, std::deque<unsigned int> &v);
unsigned int calculate_lower_bound();

void read_resources(std::ifstream &in, std::deque<unsigned int> &v);

#endif //MRP_LIBUTIL_H
