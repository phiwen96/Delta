module;
#include <coroutine>
export module Promise:Type.Interface;

// import <coroutine>;

export template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
struct promise {
	auto get_return_object () noexcept -> return_object;
	auto initial_suspend () noexcept -> initial_awaiter;
	auto final_suspend () noexcept -> final_awaiter;
	auto return_void () noexcept -> void;
	auto unhandled_exception () noexcept -> void;
};