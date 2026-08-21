# C++ Limit Order Book & Matching Engine

A C++17 implementation of an in-memory limit order book and matching engine designed to simulate core order-management and trade-execution workflows found in electronic trading systems.

The engine supports price-time priority, partial order execution, trade generation, order cancellation, file-driven order processing, and a dedicated performance benchmark.

---

## Overview

The system processes a stream of BUY and SELL orders and maintains an order book containing outstanding orders.

When a new order arrives, the matching engine checks the best available opposing orders and executes trades whenever the prices are compatible.

### High-Level Flow

```text
                    orders.txt
                        |
                        v
                +---------------+
                |  File Parser   |
                +-------+-------+
                        |
                        v
                +---------------+
                | Matching      |
                | Engine        |
                +-------+-------+
                        |
              +---------+---------+
              |                   |
              v                   v
        +-----------+       +-----------+
        | Order Book|       |  Trades   |
        +-----------+       +-----------+
                                  |
                                  v
                            trades.txt
```

---

## Features

* C++17 implementation
* Limit BUY and SELL orders
* Price-time priority
* Efficient price-level organization
* FIFO ordering for orders at the same price
* Automatic order matching
* Partial order execution
* Trade generation
* Order cancellation
* File-driven order processing
* Trade persistence to a CSV file
* Dedicated benchmark mode
* High-volume in-memory order processing

---

## Project Structure

```text
cpp-order-matching-engine/
|
├── data/
│   ├── orders.txt
│   └── trades.txt
|
├── include/
│   ├── Order.h
│   ├── OrderBook.h
│   ├── Trade.h
│   └── Benchmark.h
|
├── src/
│   ├── Order.cpp
│   ├── OrderBook.cpp
│   ├── Benchmark.cpp
│   └── main.cpp
|
├── .gitignore
└── CMakeLists.txt
```

---

## Order Book Design

The order book maintains separate BUY and SELL sides.

### BUY Side

BUY orders are sorted from highest to lowest price because a trader willing to pay more has higher execution priority.

```text
102
101
100
```

### SELL Side

SELL orders are sorted from lowest to highest price because a trader willing to sell for less has higher execution priority.

```text
103
104
105
```

### Data Structures

The implementation uses:

```text
std::map<double, std::deque<Order>>
```

for price levels.

The BUY side uses descending price ordering, while the SELL side uses ascending price ordering.

A `deque` stores orders belonging to the same price level and preserves FIFO ordering.

This gives the engine **price-time priority**:

```text
Price Priority
      |
      v
Price Level
      |
      v
FIFO Queue
      |
      v
Earlier Order Executes First
```

---

## Matching Algorithm

For an incoming BUY order:

1. Find the lowest-priced SELL order.
2. Check whether the BUY price is greater than or equal to the SELL price.
3. Determine the executable quantity.
4. Generate a trade.
5. Reduce the quantities of both orders.
6. Remove fully executed orders.
7. Continue matching until no compatible SELL order remains.
8. Place any remaining BUY quantity into the order book.

The SELL-side matching process follows the symmetric logic using the highest-priced BUY order.

### Partial Execution

For example:

```text
BUY  50 @ 100
SELL 30 @ 100
```

The engine generates:

```text
TRADE: 30 @ 100
```

and leaves:

```text
BUY 20 @ 100
```

in the order book.

---

## File-Driven Simulation

Orders can be provided through:

```text
data/orders.txt
```

Example:

```text
ADD 1 BUY LIMIT 100 50
ADD 2 BUY LIMIT 102 30
ADD 3 SELL LIMIT 101 20
ADD 4 SELL LIMIT 99 40
CANCEL 1
```

The application parses and processes these commands sequentially.

Generated trades are persisted to:

```text
data/trades.txt
```

Example output:

```text
BUY_ORDER_ID,SELL_ORDER_ID,PRICE,QUANTITY
2,3,102,20
2,4,102,10
1,4,100,30
```

---

## Running the Project

### Compile

Using `g++`:

```bash
g++ -O2 -std=c++17 -Iinclude src/main.cpp src/Order.cpp src/OrderBook.cpp src/Benchmark.cpp -o matching_engine
```

### Run the Trading Simulation

```bash
./matching_engine
```

The application reads orders from `data/orders.txt`, processes them through the matching engine, displays generated trades, and writes the trade log to `data/trades.txt`.

---

## Benchmark

The project includes a separate benchmark mode for evaluating high-volume order processing.

Run:

```bash
./matching_engine --benchmark
```

The benchmark generates a controlled workload of 100,000 orders and measures:

* Total execution time
* Order-processing throughput

Example:

```text
===== PERFORMANCE BENCHMARK =====
Orders Processed: 100000
Execution Time: 0.002857 sec
Throughput: 35001800 orders/sec
=================================
```

> Benchmark results are machine-dependent and represent a local in-memory microbenchmark rather than production trading-system performance.

---

## Complexity

Let:

* `P` = number of price levels
* `N` = number of orders at a particular price level

The ordered price map provides approximately:

```text
Price-level insertion/search: O(log P)
```

Orders at the same price are maintained in FIFO order using a deque.

Matching proceeds from the best available price level and removes fully executed orders from the front of the corresponding queue.

---

## Design Goals

The project focuses on concepts relevant to performance-oriented backend systems:

* Efficient in-memory data structures
* Deterministic order processing
* Price-time priority
* Low-overhead trade matching
* High-volume workload benchmarking
* Separation of application and benchmark execution

---

## Future Improvements

Possible extensions include:

* Market order support
* Thread-safe order ingestion
* Producer-consumer event queues
* TCP-based order submission
* Market-data event streaming
* Persistent order-book recovery
* Latency percentile measurements
* Performance profiling and optimization
* Unit and integration testing

---

## Technologies

* **C++17**
* **STL**
* **CMake**
* **Git / GitHub**
* **Linux-compatible C++ tooling**
