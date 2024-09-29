//
// Created by stefano on 28/9/24.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "book.hpp"


class engine {
    // Implement a matching engine for a stock exchange
private:
    std::unordered_map<std::string, std::unique_ptr<book>> symbol_book_map;
    std::unordered_map<int, book*> order_book_map; // still use raw pointer for fast lookup, but not ownership
    std::vector<std::string> trades;

public:
    void process_order(std::string new_order_str);

    std::vector<std::string> output_trades();
    std::vector<std::string> output_books();
};



#endif //ENGINE_H
