#include <cassert>
#include <iostream>

#include "Order.h"
#include "OrderBook.h"

void testLimitOrderMatching() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100, 50)
    );

    book.addOrder(
        Order(2, OrderSide::SELL, OrderType::LIMIT, 99, 30)
    );

    const auto& trades = book.getTrades();

    assert(trades.size() == 1);
    assert(trades[0].quantity == 30);
    assert(trades[0].buyOrderId == 1);
    assert(trades[0].sellOrderId == 2);

    std::cout << "[PASS] Limit order matching\n";
}


void testPartialFill() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100, 50)
    );

    book.addOrder(
        Order(2, OrderSide::SELL, OrderType::LIMIT, 100, 20)
    );

    const auto& trades = book.getTrades();

    assert(trades.size() == 1);
    assert(trades[0].quantity == 20);

    std::cout << "[PASS] Partial fill\n";
}


void testMarketOrder() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::SELL, OrderType::LIMIT, 105, 40)
    );

    book.addOrder(
        Order(2, OrderSide::BUY, OrderType::MARKET, 0, 25)
    );

    const auto& trades = book.getTrades();

    assert(trades.size() == 1);
    assert(trades[0].quantity == 25);
    assert(trades[0].price == 105);

    std::cout << "[PASS] Market order\n";
}


void testCancellation() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100, 50)
    );

    bool result = book.cancelOrder(1);

    assert(result == true);

    bool secondAttempt = book.cancelOrder(1);

    assert(secondAttempt == false);

    std::cout << "[PASS] Order cancellation\n";
}


void testNoMatch() {

    OrderBook book;

    book.addOrder(
        Order(1, OrderSide::BUY, OrderType::LIMIT, 100, 50)
    );

    book.addOrder(
        Order(2, OrderSide::SELL, OrderType::LIMIT, 101, 50)
    );

    const auto& trades = book.getTrades();

    assert(trades.empty());

    std::cout << "[PASS] No-match scenario\n";
}


int main() {

    std::cout << "===== RUNNING TESTS =====\n\n";

    testLimitOrderMatching();
    testPartialFill();
    testMarketOrder();
    testCancellation();
    testNoMatch();

    std::cout << "\nAll tests passed!\n";

    return 0;
}