#include "OrderBook.h"
#include <iostream>
#include <fstream>

void OrderBook::addOrder(const Order& order) {

    if (order.side == OrderSide::BUY) {
        matchBuyOrder(order);
    }
    else {
        matchSellOrder(order);
    }
}


// --------------------------------------------------
// BUY ORDER MATCHING
// --------------------------------------------------

void OrderBook::matchBuyOrder(Order order) {

    while (order.quantity > 0 &&
           !sellOrders.empty()) {

        auto bestSell = sellOrders.begin();

        double sellPrice = bestSell->first;

        // No match possible
        if (order.price < sellPrice) {
            break;
        }

        auto& sellQueue = bestSell->second;
        Order& sellOrder = sellQueue.front();

        uint64_t tradedQuantity =
            std::min(order.quantity, sellOrder.quantity);

        trades.emplace_back(
            order.id,
            sellOrder.id,
            sellPrice,
            tradedQuantity
        );

        order.quantity -= tradedQuantity;
        sellOrder.quantity -= tradedQuantity;

        if (sellOrder.quantity == 0) {
            sellQueue.pop_front();
        }

        if (sellQueue.empty()) {
            sellOrders.erase(bestSell);
        }
    }

    // Remaining quantity goes into order book
    if (order.quantity > 0) {
        buyOrders[order.price].push_back(order);
    }
}


// --------------------------------------------------
// SELL ORDER MATCHING
// --------------------------------------------------

void OrderBook::matchSellOrder(Order order) {

    while (order.quantity > 0 &&
           !buyOrders.empty()) {

        auto bestBuy = buyOrders.begin();

        double buyPrice = bestBuy->first;

        // No match possible
        if (order.price > buyPrice) {
            break;
        }

        auto& buyQueue = bestBuy->second;
        Order& buyOrder = buyQueue.front();

        uint64_t tradedQuantity =
            std::min(order.quantity, buyOrder.quantity);

        trades.emplace_back(
            buyOrder.id,
            order.id,
            buyPrice,
            tradedQuantity
        );

        order.quantity -= tradedQuantity;
        buyOrder.quantity -= tradedQuantity;

        if (buyOrder.quantity == 0) {
            buyQueue.pop_front();
        }

        if (buyQueue.empty()) {
            buyOrders.erase(bestBuy);
        }
    }

    // Remaining quantity goes into order book
    if (order.quantity > 0) {
        sellOrders[order.price].push_back(order);
    }
}


// --------------------------------------------------
// PRINT ORDER BOOK
// --------------------------------------------------

bool OrderBook::cancelOrder(uint64_t orderId) {

    // Search BUY orders
    for (auto& entry : buyOrders) {

        auto& orders = entry.second;

        for (auto it = orders.begin(); it != orders.end(); ++it) {

            if (it->id == orderId) {

                orders.erase(it);

                if (orders.empty()) {
                    buyOrders.erase(entry.first);
                }

                return true;
            }
        }
    }

    // Search SELL orders
    for (auto& entry : sellOrders) {

        auto& orders = entry.second;

        for (auto it = orders.begin(); it != orders.end(); ++it) {

            if (it->id == orderId) {

                orders.erase(it);

                if (orders.empty()) {
                    sellOrders.erase(entry.first);
                }

                return true;
            }
        }
    }

    return false;
}



void OrderBook::printBook() const {

    std::cout << "\n===== SELL ORDERS =====\n";

    for (const auto& entry : sellOrders) {

        double price = entry.first;
        const auto& orders = entry.second;

        std::cout << "Price: " << price
                  << " | Orders: " << orders.size()
                  << "\n";
    }

    std::cout << "\n===== BUY ORDERS =====\n";

    for (const auto& entry : buyOrders) {

        double price = entry.first;
        const auto& orders = entry.second;

        std::cout << "Price: " << price
                  << " | Orders: " << orders.size()
                  << "\n";
    }
}


// --------------------------------------------------
// PRINT TRADES
// --------------------------------------------------

void OrderBook::printTrades() const {

    std::cout << "\n===== TRADES =====\n";

    for (const auto& trade : trades) {

        std::cout << "BUY Order " << trade.buyOrderId
                  << " <-> SELL Order " << trade.sellOrderId
                  << " | Price: " << trade.price
                  << " | Quantity: " << trade.quantity
                  << "\n";
    }
}

void OrderBook::saveTradesToFile(const std::string& filename) const {

    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Unable to open trade log file.\n";
        return;
    }

    file << "BUY_ORDER_ID,SELL_ORDER_ID,PRICE,QUANTITY\n";

    for (const auto& trade : trades) {

        file << trade.buyOrderId << ","
             << trade.sellOrderId << ","
             << trade.price << ","
             << trade.quantity << "\n";
    }

    file.close();
}