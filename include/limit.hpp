//
// Created by stefano on 29/9/24.
//

#ifndef LIMIT_HPP
#define LIMIT_HPP

#include <list>
#include "order.hpp"


class limit {
public:
    double price;
    int tot_volume;
    std::list<std::unique_ptr<order>> orders;
};

#endif //LIMIT_HPP
