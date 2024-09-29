//
// Created by stefano on 30/9/24.
//

#include <sstream>
#include <algorithm>
#include <memory>

#include "side_limits.hpp"


// TODO: logic in this function is quite complex, consider refactoring
std::pair<std::list<std::unique_ptr<limit>>::iterator, std::list<std::unique_ptr<order>>::iterator>
side_limits::insert_order(const std::string& side, int order_id, double price, int quantity) {
    // Find or create the limit
    auto limit_iter = limits_map.find(price);
    std::list<std::unique_ptr<limit>>::iterator lim_it;
    if (limit_iter == limits_map.end()) {
        // Insert new limit in sorted order
        lim_it = limits.begin();
        if (side == "BUY") {
            while (lim_it != limits.end() && (*lim_it)->price > price) ++lim_it;
        } else {
            while (lim_it != limits.end() && (*lim_it)->price < price) ++lim_it;
        }
        lim_it = limits.insert(lim_it, std::make_unique<limit>());
        (*lim_it)->price = price;
        (*lim_it)->tot_volume = 0;
        limits_map[price] = lim_it;
    } else {
        lim_it = limit_iter->second;
    }
    // Insert order at end of limit's orders
    auto& orders = (*lim_it)->orders;
    orders.push_back(std::make_unique<order>(order_id, side, quantity, price));
    auto order_it = std::prev(orders.end());
    (*lim_it)->tot_volume += quantity;
    return {lim_it, order_it};
}


void side_limits::pull_order(std::list<std::unique_ptr<limit>>::iterator limit_it, std::list<std::unique_ptr<order>>::iterator order_it) {
    (*limit_it)->tot_volume -= (*order_it)->quantity;
    (*limit_it)->orders.erase(order_it);
    if ((*limit_it)->orders.empty()) {
        limits_map.erase((*limit_it)->price);
        limits.erase(limit_it);
    }
}


std::pair<std::list<std::unique_ptr<limit>>::iterator, std::list<std::unique_ptr<order>>::iterator>
side_limits::get_best_order() {
    if (limits.empty()) return {limits.end(), {}};
    auto lim_it = limits.begin();
    if ((*lim_it)->orders.empty()) return {limits.end(), {}};
    return {lim_it, (*lim_it)->orders.begin()};
}


std::vector<std::string> side_limits::output_levels() const {
    std::vector<std::string> levels;
    for (const auto& lim_ptr : limits) {
        int total_volume = 0;
        for (const auto& ord_ptr : lim_ptr->orders) {
            total_volume += ord_ptr->quantity;
        }
        std::ostringstream oss;
        oss << lim_ptr->price << "," << total_volume;
        levels.push_back(oss.str());
    }
    return levels;
}

std::list<std::unique_ptr<limit>>::iterator side_limits::end() {
    return limits.end();
}
