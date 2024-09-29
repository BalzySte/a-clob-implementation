//
// Created by stefano on 28/9/24.
//

#include <sstream>
#include <algorithm>
#include <memory>

#include "book.hpp"


book::book(std::string ticker) {
    this->ticker = ticker;
}


void book::insert_order(int order_id, std::string side, double price, int quantity) {
    auto& limits = (side == "BUY") ? buy_limits : sell_limits;
    auto [limit_it, order_it] = limits.insert_order(side, order_id, price, quantity);
    orders_map[order_id] = {side, limit_it, order_it};
}


void book::amend_order(int order_id, double price, int quantity) {
    auto it = orders_map.find(order_id);
    if (it == orders_map.end()) return;
    auto& ref = it->second;
    auto& limits = (ref.side == "BUY") ? buy_limits : sell_limits;
    auto& order = *(*ref.order_it);
    // If price and quantity are the same, do nothing
    if (order.price == price && order.quantity == quantity) return;
    // If price is the same but quantity is lower, reduce in place
    if (order.price == price && order.quantity > quantity) {
        (*ref.limit_it)->tot_volume -= order.quantity;
        (*ref.limit_it)->tot_volume += quantity;
        order.quantity = quantity;
        return;
    }
    // Otherwise, remove and re-insert (loses priority)
    std::string side = ref.side;
    limits.pull_order(ref.limit_it, ref.order_it);
    orders_map.erase(order_id);
    insert_order(order_id, side, price, quantity);
}


void book::pull_order(int order_id) {
    auto it = orders_map.find(order_id);
    if (it == orders_map.end()) return;
    auto& ref = it->second;
    auto& limits = (ref.side == "BUY") ? buy_limits : sell_limits;
    limits.pull_order(ref.limit_it, ref.order_it);
    orders_map.erase(order_id);
}


std::string book::match_orders(int latest_order_id) {
    auto [buy_lim_it, buy_ord_it] = buy_limits.get_best_order();
    auto [sell_lim_it, sell_ord_it] = sell_limits.get_best_order();
    if (buy_lim_it == buy_limits.end() || sell_lim_it == sell_limits.end()) return "";
    auto& buy_order = *(*buy_ord_it);
    auto& sell_order = *(*sell_ord_it);
    if (buy_order.price < sell_order.price || buy_order.quantity == 0 || sell_order.quantity == 0) return "";
    int matched_quantity = std::min(buy_order.quantity, sell_order.quantity);
    int buy_order_id = buy_order.order_id;
    int sell_order_id = sell_order.order_id;
    int aggressor_order_id = (buy_order_id == latest_order_id) ? buy_order_id : sell_order_id;
    int passive_order_id = (aggressor_order_id == buy_order_id) ? sell_order_id : buy_order_id;
    double trade_price = (passive_order_id == buy_order_id) ? buy_order.price : sell_order.price;
    if (buy_order.quantity < sell_order.quantity) {
        amend_order(sell_order.order_id, sell_order.price, sell_order.quantity - matched_quantity);
        pull_order(buy_order.order_id);
    } else if (buy_order.quantity > sell_order.quantity) {
        amend_order(buy_order.order_id, buy_order.price, buy_order.quantity - matched_quantity);
        pull_order(sell_order.order_id);
    } else {
        buy_limits.pull_order(buy_lim_it, buy_ord_it);
        sell_limits.pull_order(sell_lim_it, sell_ord_it);
        orders_map.erase(buy_order.order_id);
        orders_map.erase(sell_order.order_id);
    }
std::ostringstream oss;
    oss << this->ticker << "," << trade_price << "," << matched_quantity
        << "," << aggressor_order_id << "," << passive_order_id;
    return oss.str();
}


std::vector <std::string> book::output_book() {
    //   - bid and ask price levels (sorted best to worst) for that symbol in the format:
    //     <bid_price>,<bid_volume>,<ask_price>,<ask_volume>

    std::vector<std::string> bid_levels = buy_limits.output_levels();
    std::vector<std::string> ask_levels = sell_limits.output_levels();

    // merge the two levels strings into one, element by element
    // fill the missing elements with empty values (",")

    auto max_len = std::max(bid_levels.size(), ask_levels.size());
    std::vector<std::string> book_output;

    for (size_t i = 0; i < max_len; i++) {
        std::string bid_level = (i < bid_levels.size()) ? bid_levels[i] : ",";
        std::string ask_level = (i < ask_levels.size()) ? ask_levels[i] : ",";

        std::string book_level = bid_level + "," + ask_level;
        book_output.push_back(book_level);
    }

    return book_output;
}
