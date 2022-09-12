module;
#include <coroutine>
export module Coro:Type.Interface;

import Promise;
// import <coroutine>;

export struct coro {
	using promise_type = promise <coro, std::suspend_always, std::suspend_always>;
	explicit coro (std::coroutine_handle <promise_type>);
	coro (coro&&);
	~coro ();
private:
	std::coroutine_handle <promise_type> handle;
};