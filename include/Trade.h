#pragma once

#include <cstdint>

struct Trade {
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    double price;
    uint64_t quantity;

    Trade(uint64_t buyOrderId,
          uint64_t sellOrderId,
          double price,
          uint64_t quantity)
        : buyOrderId(buyOrderId),
          sellOrderId(sellOrderId),
          price(price),
          quantity(quantity) {}
};