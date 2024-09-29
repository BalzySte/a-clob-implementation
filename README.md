# A CLOB Implementation - Order Matching Engine

This project implements a simplified matching engine for processing trading orders. 
It manages multiple central limit order books (**CLOB**s) for different trading symbols.

## How A Matching Engine Works

A matching engine matches orders from market participants, resulting in trades when compatible buy and sell orders meet.
A trade occurs when someone wants to buy at an equal or higher price than someone else wants to sell.

### Key Concepts

- **Orders**: Each order contains an ID, side (buy/sell), limit price, and volume.
- **Limit Price**: For buy orders, the maximum price willing to pay; for sell orders, the minimum price willing to accept.
- **Central Limit Order Book (CLOB)**: Contains two sides (buy and sell) with all active orders.
- **Matching Priority**: 
  1. Price priority: Best price matches first
  2. Time priority: For same price, earlier orders match first
- **Trade Generation**: When a match occurs, a trade is generated with:
  - Aggressive order: The order causing the trade (new or amended)
  - Passive order: The matching order on the opposite side
  - Trade volume: Minimum of the active volumes of matched orders
  - Trade price: Determined by the price of the passive order

### Order Operations

- **Insert**: Add a new order to the book
- **Pull**: Remove an order from the book
- **Amend**: Change price and/or volume of an order
  - Amending causes loss of time priority unless only decreasing volume
  - Price amendments require re-evaluation for potential matches

## Features

- Parse input orders from text (typically via terminal input or redirected file)
- Match buy and sell orders according to price-time priority
- Support for order amendments and cancellations
- Output details of trades that occurred
- Output snapshots of order books for each traded security

## Project Structure

```
order_match_engine/
  ├── include/              # Header files
  │    └── *.hpp
  ├── src/                  # Source files (engine logic)
  │    ├── *.cpp
  │    └── main.cpp
  ├── tests/                # Tests for the engine
  │    └── tests.cpp
  ├── CMakeLists.txt        # Build configuration
  └── README.md             # Original assignment description
```

## Input Format

Each input line represents a single order and should follow a format with order ID, side, price, and quantity:

```
<order_id> <side: buy/sell> <price> <quantity>
```

Example:

```
10001 buy 101.5 10
10002 sell 101.5 5
```

## Output

After all input orders are processed, the application prints:

1. **Executed trade information**: Order IDs, price, quantity
2. **Final state of each order book**: Remaining buy/sell orders

## How to Build

This project uses CMake:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

To run interactively:

```bash
./order_match_engine
```

Or with an input file:

```bash
./order_match_engine < input.txt
```

## Testing

Tests are located in `tests/tests.cpp` and implemented with Catch2.
To run the tests, ensure you have Catch2 installed and then build the project as described above.
After building, execute:

```bash
./tests
```

---
