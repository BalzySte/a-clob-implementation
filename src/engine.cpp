//
// Created by stefano on 28/9/24.
//


#include <algorithm>
#include <sstream>
#include <string>
#include <memory>

#include "engine.hpp"
#include "book.hpp"


void engine::process_order(std::string new_order_str) {
    // SAMPLE: "INSERT,1,AAPL,BUY,12.2,5"

    // replace commas with spaces to make parsing easier
    std::replace(new_order_str.begin(), new_order_str.end(), ',', ' ');

    // parse the oder string to first figure out the acion (INSERT, AMEND, PULL)
    std::istringstream iss(new_order_str);
    std::string action;
    iss >> action;

    // different actions come with different number of arguments
    bool check_for_match = false;
    book* book_ptr = nullptr;
    int order_id;

    if (action == "INSERT") {
        std::string symbol;
        std::string side;
        double price;
        int quantity;

        iss >> order_id >> symbol >> side >> price >> quantity;

        // if a book for the symbol does not exist, create a new book
        auto book_iter = symbol_book_map.find(symbol);
        if (book_iter == symbol_book_map.end()) {
            auto new_book = std::make_unique<book>(symbol);
            book_ptr = new_book.get();
            symbol_book_map[symbol] = std::move(new_book);
            book_iter = symbol_book_map.find(symbol);
        }

        // get the book for the symbol
        book_ptr = book_iter->second.get();

        // update the order_book_map with the order_id and the book
        order_book_map[order_id] = book_ptr;

        // perform the insert operation on the book
        book_ptr->insert_order(order_id, side, price, quantity);

        check_for_match = true;

    } else if (action == "AMEND") {
        double price;
        int quantity;

        iss >> order_id >> price >> quantity;

        // find the book that contains the order
        auto book_iter = order_book_map.find(order_id);
        if (book_iter == order_book_map.end()) {
            throw;
        }

        book_ptr = book_iter->second;

        book_ptr->amend_order(order_id, price, quantity);

        check_for_match = true;

    } else if (action == "PULL") {
        iss >> order_id;

        // find the book that contains the order
        auto book_iter = order_book_map.find(order_id);
        if (book_iter == order_book_map.end()) {
            throw;
        }

        book_ptr = book_iter->second;

        book_ptr->pull_order(order_id);

        // remove the order from the order_book_map
        order_book_map.erase(order_id);

        // matching is not necessary for a pull operation
    }

    // NOTE: some amend cases do not require to check for matches, not optimizing for that for now
    if (not check_for_match) {
        return;
    }

    // do match orders while trades are returned
    // int count = 0;
    do {
        std::string trades_str = book_ptr->match_orders(order_id);
        if (trades_str.empty()) break;
        trades.push_back(trades_str);
        // count += 1;
    } while (true);
    //} while (count < 10);  // NOTE: use a hard limit to avoid infinite loops while debugging
}


std::vector<std::string> engine::output_trades() {
    return trades;
}


std::vector<std::string> engine::output_books() {
    // add the book snapshots to the output, per symbol, in alphabetical order
    // separated by: ===<symbol>===

    // get all the symbols
    std::vector<std::string> symbols;
    for (auto const& pair : symbol_book_map) {
        symbols.push_back(pair.first);
    }

    // sort the symbols
    std::sort(symbols.begin(), symbols.end());

    // iterate over the symbols and build the output
    std::vector<std::string> books_output;

    for (auto const& symbol : symbols) {
        book* book_ptr = symbol_book_map[symbol].get();

        std::vector<std::string> book_output;

        // add the symbol header to the book output
        std::string book_str = "===" + symbol + "===";
        book_output.push_back(book_str);

        // add the book snapshot to the book output (bid and ask levels)
        std::vector<std::string> book_snapshot = book_ptr->output_book();
        book_output.insert(book_output.end(), book_snapshot.begin(), book_snapshot.end());

        // add the book_output to the books_output
        books_output.insert(books_output.end(), book_output.begin(), book_output.end());
    }

    return books_output;
}
