//
// Created by stefano on 28/9/24.
//

#include <string>
#include <sstream>

#include "order.hpp"


order::order(int order_id, std::string side, int quantity, double price) {
    // this->symbol = symbol;
    this->order_id = order_id;
    this->side = side;
    this->quantity = quantity;
    this->price = price;
}
