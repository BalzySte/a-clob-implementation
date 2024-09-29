#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "run.hpp"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file


TEST_CASE("insert") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===AAPL===");
    CHECK(result[1] == "12.2,5,,");
}

TEST_CASE("simple match sell is aggressive") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");
    input.emplace_back("INSERT,2,AAPL,SELL,12.1,8");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "AAPL,12.2,5,2,1");
    CHECK(result[1] == "===AAPL===");
    CHECK(result[2] == ",,12.1,3");
}

TEST_CASE("simple match buy is aggressive") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,SELL,12.1,8");
    input.emplace_back("INSERT,2,AAPL,BUY,12.2,5");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "AAPL,12.1,5,2,1");
    CHECK(result[1] == "===AAPL===");
    CHECK(result[2] == ",,12.1,3");
}

TEST_CASE("multi insert and multi match") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,8,AAPL,BUY,14.235,5");
    input.emplace_back("INSERT,6,AAPL,BUY,14.235,6");
    input.emplace_back("INSERT,7,AAPL,BUY,14.235,12");
    input.emplace_back("INSERT,2,AAPL,BUY,14.234,5");
    input.emplace_back("INSERT,1,AAPL,BUY,14.23,3");
    input.emplace_back("INSERT,5,AAPL,SELL,14.237,8");
    input.emplace_back("INSERT,3,AAPL,SELL,14.24,9");
    input.emplace_back("PULL,8");
    input.emplace_back("INSERT,4,AAPL,SELL,14.234,25");

    auto result = run(input);

    REQUIRE(result.size() == 7);
    CHECK(result[0] == "AAPL,14.235,6,4,6");
    CHECK(result[1] == "AAPL,14.235,12,4,7");
    CHECK(result[2] == "AAPL,14.234,5,4,2");
    CHECK(result[3] == "===AAPL===");
    CHECK(result[4] == "14.23,3,14.234,2");
    CHECK(result[5] == ",,14.237,8");
    CHECK(result[6] == ",,14.24,9");
}

TEST_CASE("multi symbol") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,0.3854,5");
    input.emplace_back("INSERT,2,TSLA,BUY,412,31");
    input.emplace_back("INSERT,3,TSLA,BUY,410.5,27");
    input.emplace_back("INSERT,4,AAPL,SELL,21,8");
    input.emplace_back("INSERT,11,WEBB,SELL,0.3854,4");
    input.emplace_back("INSERT,13,WEBB,SELL,0.3853,6");

    auto result = run(input);

    REQUIRE(result.size() == 9);
    CHECK(result[0] == "WEBB,0.3854,4,11,1");
    CHECK(result[1] == "WEBB,0.3854,1,13,1");
    CHECK(result[2] == "===AAPL===");
    CHECK(result[3] == ",,21,8");
    CHECK(result[4] == "===TSLA===");
    CHECK(result[5] == "412,31,,");
    CHECK(result[6] == "410.5,27,,");
    CHECK(result[7] == "===WEBB===");
    CHECK(result[8] == ",,0.3853,5");
}


TEST_CASE("pull first order in limit") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,1");
    input.emplace_back("INSERT,2,WEBB,BUY,45.95,2");
    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===WEBB===");
    CHECK(result[1] == "45.95,2,,");
}


TEST_CASE("basic amend") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,1");
    input.emplace_back("INSERT,2,WEBB,BUY,45.95,2");
    input.emplace_back("AMEND,1,45.95,3");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===WEBB===");
    CHECK(result[1] == "45.95,5,,");
}


TEST_CASE("amend") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,5");
    input.emplace_back("INSERT,2,WEBB,BUY,45.95,6");
    input.emplace_back("INSERT,3,WEBB,BUY,45.95,12");
    input.emplace_back("INSERT,4,WEBB,SELL,46,8");
    input.emplace_back("AMEND,2,46,3");
    input.emplace_back("INSERT,5,WEBB,SELL,45.95,1");
    input.emplace_back("AMEND,1,45.95,3");
    input.emplace_back("INSERT,6,WEBB,SELL,45.95,1");
    input.emplace_back("AMEND,1,45.95,5");
    input.emplace_back("INSERT,7,WEBB,SELL,45.95,1");

    auto result = run(input);

    REQUIRE(result.size() == 6);
    CHECK(result[0] == "WEBB,46,3,2,4");
    CHECK(result[1] == "WEBB,45.95,1,5,1");
    CHECK(result[2] == "WEBB,45.95,1,6,1");
    CHECK(result[3] == "WEBB,45.95,1,7,3");
    CHECK(result[4] == "===WEBB===");
    CHECK(result[5] == "45.95,16,46,5");
}


TEST_CASE("basic pull") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,5");
    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 1);
    CHECK(result[0] == "===WEBB===");
}


TEST_CASE("pull with another order") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,5");
    input.emplace_back("INSERT,2,WEBB,SELL,46,1");
    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===WEBB===");
    CHECK(result[1] == ",,46,1");
}


TEST_CASE("pull") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,WEBB,BUY,45.95,5");
    input.emplace_back("INSERT,2,WEBB,BUY,46,6");
    input.emplace_back("INSERT,3,WEBB,SELL,46,8");
    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "WEBB,46,6,3,2");
    CHECK(result[1] == "===WEBB===");
    CHECK(result[2] == ",,46,2");
}


TEST_CASE("a complex case") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,3,TSLA,SELL,500,10");
    input.emplace_back("INSERT,4,MSFT,SELL,10,8");
    input.emplace_back("INSERT,11,MSFT,SELL,10,4");
    input.emplace_back("INSERT,5,MSFT,SELL,11,8"); // should match second and leave 3 in the book
    input.emplace_back("AMEND,4,10,5");
    input.emplace_back("INSERT,6,MSFT,BUY,10.1,6");
    input.emplace_back("INSERT,9,TSLA,BUY,490,7");
    input.emplace_back("INSERT,10,TSLA,SELL,510,5");
    input.emplace_back("AMEND,3,480,8");

    auto result = run(input);

    // TSLA:
    // * 1 trade @ 490 qty 7
    // * book buy side: 480 qty 1 + 510 qty 5
    // MSFT:
    // * 1 trade @ 10 qty 5
    // * book sell side: 10 qty 3 + 11 qty 8

    REQUIRE(result.size() == 9);
    CHECK(result[0] == "MSFT,10,5,6,4");
    CHECK(result[1] == "MSFT,10,1,6,11");
    CHECK(result[2] == "TSLA,490,7,3,9");
    CHECK(result[3] == "===MSFT===");
    CHECK(result[4] == ",,10,3");
    CHECK(result[5] == ",,11,8");
    CHECK(result[6] == "===TSLA===");
    CHECK(result[7] == ",,480,1");
    CHECK(result[8] == ",,510,5");
}