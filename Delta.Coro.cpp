export module Coro;
// export struct coro;
import <coroutine>;

export import :Promise;


// export struct coro {
// 	coro (coro && other) noexcept;
// 	~coro ();
// 	auto operator co_await () && noexcept;
// private:
// 	explicit coro (std::coroutine_handle <promise_type>);
// 	std::coroutine_handle <promise_type> handle;
// };