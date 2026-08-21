#include "Benchmark.h"
#include "OrderBook.h"
#include "Order.h"

#include <iostream>
#include <chrono>

void runBenchmark() {

    const int NUM_ORDERS = 100000;

    OrderBook book;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; i++) {

        if (i % 2 == 0) {

            book.addOrder(
                Order(
                    i,
                    OrderSide::BUY,
                    OrderType::LIMIT,
                    100.0,
                    10
                )
            );
        }
        else {

            book.addOrder(
                Order(
                    i,
                    OrderSide::SELL,
                    OrderType::LIMIT,
                    101.0,
                    10
                )
            );
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start);

    double seconds = duration.count() / 1'000'000.0;

    std::cout << "\n===== PERFORMANCE BENCHMARK =====\n";

    std::cout << "Orders Processed: "
              << NUM_ORDERS << "\n";

    std::cout << "Execution Time: "
              << seconds
              << " sec\n";

    if (seconds > 0) {

        std::cout << "Throughput: "
                  << NUM_ORDERS / seconds
                  << " orders/sec\n";
    }

    std::cout << "=================================\n";
}