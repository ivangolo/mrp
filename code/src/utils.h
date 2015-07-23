//
// Created by ivanedo on 12-06-15.
//

#ifndef MRP_LIBUTIL_H
#define MRP_LIBUTIL_H

#include <deque>
#include <map>
#include "sets.h"

struct CompareSecond {
    template <typename T>
    bool operator()(const T& pLhs, const T& pRhs) {
        return pLhs.second < pRhs.second;
    }
};
void show_help_info(char *s);
std::pair<unsigned int, int64_t> get_min(SolutionNeighborhood neighborhood);


#endif //MRP_LIBUTIL_H
