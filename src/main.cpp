#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Order.h"
#include "OrderBook.h"
#include "Benchmark.h"

int main(int argc, char* argv[]) {
     // -----------------------------------------
    // BENCHMARK MODE
    // -----------------------------------------

    if (argc > 1 &&
        std::string(argv[1]) == "--benchmark") {

        runBenchmark();
        return 0;
    }


    // -----------------------------------------
    // NORMAL TRADING MODE
    // -----------------------------------------

    OrderBook book;

    std::ifstream file("data/orders.txt");

    if (!file) {
        std::cerr << "Unable to open orders.txt\n";
        return 1;
    }

    std::string line;

    std::cout << "===== PROCESSING ORDERS =====\n\n";

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);

        std::string command;
        ss >> command;

        if (command == "ADD") {

            uint64_t id;
            std::string side;
            std::string type;
            double price;
            uint64_t quantity;

            ss >> id >> side >> type >> price >> quantity;

            OrderSide orderSide =
                (side == "BUY")
                ? OrderSide::BUY
                : OrderSide::SELL;

            OrderType orderType =
                (type == "LIMIT")
                ? OrderType::LIMIT
                : OrderType::MARKET;

            std::cout
                << "Adding "
                << side
                << " Order " << id
                << " | Price: " << price
                << " | Quantity: " << quantity
                << "\n";

            book.addOrder(
                Order(
                    id,
                    orderSide,
                    orderType,
                    price,
                    quantity
                )
            );
        }

        else if (command == "CANCEL") {

            uint64_t id;

            ss >> id;

            bool cancelled = book.cancelOrder(id);

            std::cout
                << "Cancelling Order "
                << id
                << " -> "
                << (cancelled ? "SUCCESS" : "FAILED")
                << "\n";
        }
    }

    file.close();

    std::cout << "\n";
    book.printTrades();

    std::cout << "\n";
    book.printBook();

    book.saveTradesToFile("data/trades.txt");

    std::cout
        << "\nTrade log saved to data/trades.txt\n";

    return 0;
}