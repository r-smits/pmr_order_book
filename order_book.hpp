#pragma once
#include <iostream>
#include <list>
#include <map>
#include <memory_resource>

struct Order {

  u_int64_t id;
  double quantity;
  double price;
  bool is_buy;
};

using bid_map = std::map<double, std::pmr::list<Order>, std::greater<double>,
                         std::pmr::polymorphic_allocator<
                             std::pair<const double, std::pmr::list<Order>>>>;

using ask_map = std::map<double, std::pmr::list<Order>, std::less<double>,
                         std::pmr::polymorphic_allocator<
                             std::pair<const double, std::pmr::list<Order>>>>;

class OrderBook {

private:
  bid_map bids;
  ask_map asks;
  std::pmr::unordered_map<u_int64_t, std::pmr::list<Order>::iterator> indices;

public:
  OrderBook(std::pmr::monotonic_buffer_resource *arena);
  bool place_limit_order(const Order &limit_order);
  bool cancel_limit_order(const Order &cancel_order);
  bool match_limit_order(Order &limit_order);
  bool print_order_book();
};
