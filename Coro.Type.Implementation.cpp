module;
#include <coroutine>
#include <utility>
module Coro:Type.Implementation;

// import <utility>;
// import <coroutine>;

import :Type.Interface;

coro::coro (coro&& other) : handle (std::exchange (other.handle, {})) {

}
coro::~coro () {
	if (handle) {
		handle.destroy ();
	}
}
coro::coro (std::coroutine_handle <promise_type> handle) : handle {handle} {

}