#pragma once

#include <cstdint>

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

struct Order {
    uint64_t id;
    OrderSide side;
    OrderType type;
    double price;
    uint64_t quantity;

    Order(uint64_t id,
          OrderSide side,
          OrderType type,
          double price,
          uint64_t quantity)
        : id(id),
          side(side),
          type(type),
          price(price),
          quantity(quantity) {}
};