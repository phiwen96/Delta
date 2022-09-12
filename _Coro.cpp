module Coro;

import Coro.Promise;

import <iostream>;
import <coroutine>;
import <utility>;

struct coro {
	using promise_type = promise <coro, std::suspend_always, std::suspend_always>;
	coro (coro && other) noexcept : handle {std::exchange (other.handle, {})} {

}
	~coro (){
	if (handle) {
		handle.destroy ();
	}
}

	auto operator co_await () && noexcept{
	return std::suspend_always {};
}

	explicit coro (std::coroutine_handle <promise_type>){

}
private:
	std::coroutine_handle <promise_type> handle;
};

// coro::coro (coro && other) noexcept : handle {std::exchange (other.handle, {})} {

// }
// coro::~coro () {
// 	if (handle) {
// 		handle.destroy ();
// 	}
// }
// auto coro::operator co_await () && noexcept {
// 	return std::suspend_always {};
// }
// coro::coro (std::coroutine_handle <promise_type> handle) : handle {handle} {

// }