module;
#include <coroutine>
export module Coro;

// export import <coroutine>;
export import :Type;


// import <iostream>;
// export import <coroutine>;
// import <utility>;
// export struct coro;


// export import :Promise;

// export struct bajs {auto kiss () -> void;};

// export template <typename T>
// concept Coroutine = requires (T coro) {
// 	coro.resume();
// 	coro.done();
// 	coro.alive();
// };




// export {struct coro {
// 	// using promise_type = promise <coro, std::suspend_always, std::suspend_always>;
// 	coro (coro && other) noexcept;
// 	// ~coro ();

// 	// auto operator co_await () && noexcept;

// 	// explicit coro (std::coroutine_handle <promise_type>);
// private:
// 	// std::coroutine_handle <promise_type> handle;
// };

// }

// module :private;
// coro::coro (coro && other) noexcept {

// }
// export struct coro {
// 	using promise_type = promise <coro, std::suspend_always, std::suspend_always>;
// 	coro (coro && other) noexcept : handle {std::exchange (other.handle, {})} {

// }
// 	~coro (){
// 	if (handle) {
// 		handle.destroy ();
// 	}
// }

// 	auto operator co_await () && noexcept{
// 	return std::suspend_always {};
// }

// 	explicit coro (std::coroutine_handle <promise_type>) : handle {handle} {

// }
// private:
// 	std::coroutine_handle <promise_type> handle;
// };

