module;
#include <coroutine>
#include <utility>
#include <algorithm>
module Coro:Type.Implementation;

// import <utility>;
// import <coroutine>;

import :Type.Interface;
import Awaitable;
import Promise;

template <Awaitable initial_awaitable>
auto coro <initial_awaitable>::await_ready () noexcept -> bool {
	return false;
}

template <Awaitable initial_awaitable>
auto coro <initial_awaitable>::await_suspend (std::coroutine_handle <> c) noexcept -> std::coroutine_handle <promise_type> {
	handle.promise().continuation = c;
	return handle;
}

template <Awaitable initial_awaitable>
auto coro <initial_awaitable>::await_resume () noexcept -> void {

}

// template <Awaitable initial_awaitable>
// coro <initial_awaitable>::coro (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} 
// {
// 	// std::swap (handle, h);
// }

template <Awaitable initial_awaitable>
coro <initial_awaitable>::coro (coro&& other) noexcept : handle (std::exchange (other.handle, {}))
{

}
template <Awaitable initial_awaitable>
coro <initial_awaitable>::~coro () {
	if (handle) {
		handle.destroy ();
	}
}
