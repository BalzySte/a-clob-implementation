//
// Created by stefano on 30/9/24.
//

#ifndef SIDE_LIMITS_HPP
#define SIDE_LIMITS_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <memory>

#include "limit.hpp"


class side_limits {
    // Implement structures to represent the limits on both sides of the order book
private:
    // Map price to iterator in limits list
    std::unordered_map<double, std::list<std::unique_ptr<limit>>::iterator> limits_map;
    std::list<std::unique_ptr<limit>> limits;

public:
    // Insert order, returns iterator to the order in the limit's orders list
    std::pair<std::list<std::unique_ptr<limit>>::iterator, std::list<std::unique_ptr<order>>::iterator>
    insert_order(const std::string& side, int order_id, double price, int quantity);

    // Remove order by limit and order iterator
    void pull_order(std::list<std::unique_ptr<limit>>::iterator limit_it, std::list<std::unique_ptr<order>>::iterator order_it);

    // Get best order (returns pair of iterators)
    std::pair<std::list<std::unique_ptr<limit>>::iterator, std::list<std::unique_ptr<order>>::iterator> get_best_order();

    std::vector<std::string> output_levels() const;

    // Expose end iterator for comparison
    std::list<std::unique_ptr<limit>>::iterator end();
};

#endif //SIDE_LIMITS_HPP
