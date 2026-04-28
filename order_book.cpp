#include "order_book.hpp"
#include <memory_resource>

OrderBook::OrderBook(std::pmr::monotonic_buffer_resource *arena)
    : bids(arena), asks(arena), indices(arena) {};

std::ostream &operator<<(std::ostream &outs, const Order &order) {
  return outs << "ORDER = {.id = " << order.id << ", .price = " << order.price
              << ", .quantity = " << order.quantity
              << ", .is_buy = " << order.is_buy << "}";
}

bool OrderBook::print_order_book() {
  std::cout << "-- BIDS --" << std::endl;
  for (auto [price, order_list] : bids) {
    for (const Order &order : order_list) {
      std::cout << order << std::endl;
    }
  }
  std::cout << std::endl;
  std::cout << "-- ASKS -- " << std::endl;
  for (auto [price, order_list] : asks) {
    for (const Order &order : order_list) {
      std::cout << order << std::endl;
    }
  }
  std::cout << std::endl;
  std::cout << std::endl;
  return true;
};

template <typename T>
void add_to_map(
    std::pmr::map<double, std::pmr::list<Order>, T> &map,
    std::pmr::unordered_map<u_int64_t, std::pmr::list<Order>::iterator>
        &indices,
    const Order &limit_order) {
  map[limit_order.price].push_back(limit_order);
  indices[limit_order.id] = map[limit_order.price].begin();
};

template <typename T>
void match_with_map(std::pmr::map<double, std::pmr::list<Order>, T> &map,
                    Order &order) {
  T comparator;
  auto map_it = map.begin();
  while (map_it != map.end() && order.quantity > 0) {
    std::pmr::list<Order>& order_it = map_it->second;
    while (!order_it.empty() && order.quantity > 0 &&
           (comparator(order_it.front().price, order.price) ||
            order_it.front().price == order.price)) {
      Order &best = order_it.front();
      int remaining_quantity = order.quantity - best.quantity;
      if (order.quantity >= best.quantity)
        order_it.pop_front();
			best.quantity -= order.quantity;
      order.quantity = std::max(0, remaining_quantity);
    }
		++map_it;
  }
}

bool OrderBook::place_limit_order(const Order &order) {
  (order.is_buy) ? add_to_map(bids, indices, order)
                 : add_to_map(asks, indices, order);
  return true;
};

bool OrderBook::cancel_limit_order(const Order &limit_order) {
  if (limit_order.is_buy) {
    bids[limit_order.price].erase(indices[limit_order.id]);
    if (bids[limit_order.price].empty())
      bids.erase(limit_order.price);
  } else {
    asks[limit_order.price].erase(indices[limit_order.id]);
    if (asks[limit_order.price].empty())
      asks.erase(limit_order.price);
  }
  indices.erase(limit_order.id);
  return true;
};

bool OrderBook::match_limit_order(Order &limit_order) {
	Order& order = *indices[limit_order.id];
  (order.is_buy) ? match_with_map(asks, order) : match_with_map(bids, order);
  return true;
};
