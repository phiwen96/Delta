module;
#include <coroutine>
export module Promise:Type.Interface;

import Awaitable;

// import <coroutine>;

export template <
	typename return_object,
	Awaitable initial_awaiter>
struct promise {
	auto get_return_object () noexcept -> return_object;
	auto initial_suspend () noexcept -> initial_awaiter;
	auto final_suspend () noexcept -> auto;
	auto return_void () noexcept -> void;
	auto unhandled_exception () noexcept -> void;
private:
	std::coroutine_handle <> continuation;
};