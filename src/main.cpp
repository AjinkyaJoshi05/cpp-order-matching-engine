#include <iostream>
#include <chrono>

#include "Order.h"
#include "OrderBook.h"

int main() {

    OrderBook book;

    const int NUM_ORDERS = 100000;

    auto start =
        std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; i++) {

        if (i % 2 == 0) {

            book.addOrder(
                Order(
                    i,
                    OrderSide::BUY,
                    OrderType::LIMIT,
                    100,
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
                    100,
                    10
                )
            );
        }
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(end - start);

    double seconds =
        duration.count() / 1000.0;

    std::cout
        << "Orders Processed: "
        << NUM_ORDERS
        << "\n";

    std::cout
        << "Execution Time: "
        << seconds
        << " sec\n";

    std::cout
        << "Throughput: "
        << (NUM_ORDERS / seconds)
        << " orders/sec\n";

    return 0;
}