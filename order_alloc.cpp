#include "order_book.hpp"
#include <memory_resource>

constexpr size_t MB_2 = 1024 * 1024 * 2;

int main() {

  std::array<std::byte, MB_2> buffer;
  std::pmr::monotonic_buffer_resource arena{buffer.data(), buffer.size()};

  OrderBook order_book{&arena};

  Order order_1{.id = 1, .quantity = 3, .price = 20.5, .is_buy = true};
	Order order_2{.id = 2, .quantity = 1, .price = 16.5, .is_buy = false};
	Order order_3{.id = 3, .quantity = 6, .price = 50.3, .is_buy = true};
	Order order_4{.id = 4, .quantity = 4, .price = 34.8, .is_buy = false};
	Order order_5{.id = 5, .quantity = 8, .price = 20.4, .is_buy = true};
	
	order_book.place_limit_order(order_1);
	order_book.place_limit_order(order_2);
	order_book.place_limit_order(order_3);
	order_book.place_limit_order(order_4);
	order_book.place_limit_order(order_5);

	order_book.print_order_book();

	order_book.cancel_limit_order(order_2);

	order_book.print_order_book();

	order_book.match_limit_order(order_3);

	order_book.print_order_book();
	return 0;
};
