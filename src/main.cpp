#include <bits/stdc++.h>

#include "Order.h"
#include "OrderBook.h"

int main() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100.0, 50)
    );

    book.addOrder(
        Order(2, OrderSide::BUY, OrderType::LIMIT, 102.0, 30)
    );

    book.addOrder(
        Order(3, OrderSide::SELL, OrderType::LIMIT, 105.0, 40)
    );

    std::cout << "\nBEFORE CANCELLATION";
    book.printBook();

    bool cancelled = book.cancelOrder(2);

    std::cout << "\nCancellation result: "
              << (cancelled ? "SUCCESS" : "FAILED")
              << "\n";

    std::cout << "\nAFTER CANCELLATION";
    book.printBook();

    return 0;
}