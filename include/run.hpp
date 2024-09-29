#pragma once

#include <string>
#include <vector>

// Run the strategy engine for a list of input commands and returns the trades and orderbooks in a
// csv-like format. Every command starts with either "INSERT", "AMEND" or "PULL" with additional
// data in the columns after the command.
//
// In case of insert the line will have the format:
// INSERT,<order_id>,<symbol>,<side>,<price>,<volume>
// e.g. INSERT,4,AAPL,BUY,23.45,12
//
// In case of amend the line will have the format:
// AMEND,<order_id>,<price>,<volume>
// e.g. AMEND,4,23.12,11
//
// In case of pull the line will have the format:
// <PULL>,<order_id>
// e.g. PULL,4
//
// Side will always be "BUY" or "SELL".
// A price is a string with a maximum of 4 digits behind the ".", so "2.1427" and "33.42" would be
// valid prices but "2.14275" would not be a valid price since it has more than 4 digits behind the
// comma.
// A volume will be an integer
//
// The expected output is:
// - List of trades in chronological order with the format:
//   <symbol>,<price>,<volume>,<aggressive_order_id>,<passive_order_id>
//   e.g. AAPL,23.55,11,4,7
// - Then, per symbol (in alphabetical order):
//   - separator "===<symbol>==="
//   - bid and ask price levels (sorted best to worst) for that symbol in the format:
//     <bid_price>,<bid_volume>,<ask_price>,<ask_volume>
//     e.g. 25.52,23,25.56,34
//          25.51,11,25.67,102
//          25.43,4,,

std::vector<std::string> run(std::vector<std::string> const& input);
