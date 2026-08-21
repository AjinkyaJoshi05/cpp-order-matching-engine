#include <iostream>

#include "Order.h"
#include "OrderBook.h"

int main() {

    OrderBook book;

    // BUY 50 shares @ 100
    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100.0, 50)
    );

    // SELL 30 shares @ 99
    // This should match with BUY Order 1
    book.addOrder(
        Order(2, OrderSide::SELL, OrderType::LIMIT, 99.0, 30)
    );

    book.printTrades();
    book.printBook();

    return 0;
}