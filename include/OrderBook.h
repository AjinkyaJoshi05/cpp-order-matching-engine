#pragma once

#include "Order.h"
#include "Trade.h"

#include <map>
#include <deque>
#include <vector>
#include <string>

class OrderBook {
private:

    // Highest BUY price first
    std::map<double, std::deque<Order>, std::greater<double>> buyOrders;

    // Lowest SELL price first
    std::map<double, std::deque<Order>> sellOrders;

    std::vector<Trade> trades;

    void matchBuyOrder(Order order);

    void matchSellOrder(Order order);

    void matchMarketBuy(Order order);   
    void matchMarketSell(Order order);

public:

    void addOrder(const Order& order);

    bool cancelOrder(uint64_t orderId);

    void printBook() const;

    void printTrades() const;

    void saveTradesToFile(const std::string& filename) const;
    
    const std::vector<Trade>& getTrades() const;
};