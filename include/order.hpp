//
// Created by stefano on 28/9/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <string>


class limit;


class order {
    // Implement an order class for a stock exchange
public:
    int order_id;

    std::string side;
    int quantity;
    double price;
    // No parent_limit or linked list pointers needed
    order(int order_id, std::string side, int quantity, double price);
};

#endif //ORDER_H
