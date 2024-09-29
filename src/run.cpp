//
// Created by stefano on 28/9/24.
//

#include <iostream>
#include <string>

#include "run.hpp"
#include "engine.hpp"


std::vector<std::string> run(std::vector<std::string> const& input) {
    engine engine;

    // run the engine for each line of input and store the output
    std::vector<std::string> output;

    for (const std::string& line : input) {
        // output.push_back(engine.receive_order(line));
        engine.process_order(line);
    }

    std::vector<std::string> books_output = engine.output_books();

    // insert the trades output
    std::vector<std::string> trades_output = engine.output_trades();
    output.insert(output.end(), trades_output.begin(), trades_output.end());

    // extend the output with the books snapshots
    output.insert(output.end(), books_output.begin(), books_output.end());

    // print the output
    for (const std::string& line : output) {
        std::cout << line << std::endl;
    }

    // return the output
    return output;
}
