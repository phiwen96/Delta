module;
#include <coroutine>
export module Coro:Type.Interface;

import Promise;
import Awaitable;
// import <coroutine>;

export template <Awaitable initial_awaitable>
struct coro {
	using promise_type = promise <coro, initial_awaitable>;
	auto await_ready () noexcept -> bool;
	auto await_suspend (std::coroutine_handle <>) noexcept -> std::coroutine_handle <promise_type>;
	auto await_resume () noexcept -> void;
	coro (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	coro (coro&&) noexcept;
	~coro ();
private:
	std::coroutine_handle <promise_type> handle;
};

