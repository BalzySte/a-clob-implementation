//
// Created by stefano on 28/9/24.
//

#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <list>

#include "order.hpp"
#include "side_limits.hpp"


class book {
private:
    std::string ticker;

    side_limits buy_limits;
    side_limits sell_limits;
    // Map order_id to tuple: side, limit iterator, order iterator
    struct OrderRef {
        std::string side;
        std::list<std::unique_ptr<limit>>::iterator limit_it;
        std::list<std::unique_ptr<order>>::iterator order_it;
    };
    std::unordered_map<int, OrderRef> orders_map;

    double best_buy;  // best bid
    double best_sell; // best ask

public:
    book(std::string ticker);

    void insert_order(int order_id, std::string side, double price, int quantity);
    void amend_order(int order_id, double price, int quantity);
    void pull_order(int order_id);

    std::string match_orders(int latest_order_id);

    std::vector <std::string> output_book();
};

#endif //BOOK_H
