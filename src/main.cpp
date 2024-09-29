//
// Created by stefano on 29/9/24.
//

#include <iostream>
#include <string>

#include "run.hpp"
#include "engine.hpp"


int main() {
    std::vector<std::string> input;
    std::vector<std::string> output;
    std::string line;

    while (std::getline(std::cin, line)) {
        input.push_back(line);
    }

    output = run(input);

    for (const std::string& line : output) {
        std::cout << line << std::endl;
    }

    return 0;
}
