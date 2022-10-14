module;
#include <coroutine>
#include <stdio.h>
#include <stdlib.h>
module Promise:Type.Implementation;

import :Type.Interface;
import Awaitable;


template <typename return_object, Awaitable initial_awaiter>
auto promise <return_object, initial_awaiter>::get_return_object () noexcept -> return_object {
	// return return_object {std::coroutine_handle <promise>::from_promise (*this)};
	return return_object {*this};
}

template <typename return_object, Awaitable initial_awaiter>
auto promise <return_object, initial_awaiter>::initial_suspend () noexcept -> initial_awaiter {
	return {};
}

template <typename return_object, Awaitable initial_awaiter>
auto promise <return_object, initial_awaiter>::final_suspend () noexcept -> auto {
	struct awaitable {
		auto await_ready () noexcept -> bool {
				return false;
		}
		auto await_suspend (std::coroutine_handle <promise> h) noexcept -> std::coroutine_handle <> {
			return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
		}
		auto await_resume () noexcept -> void {

		}
	};
	return awaitable {};
}

template <typename return_object, Awaitable initial_awaiter>
auto promise <return_object, initial_awaiter>::return_void () noexcept -> void {

}

template <typename return_object, Awaitable initial_awaiter>
auto promise <return_object, initial_awaiter>::unhandled_exception () noexcept -> void {
	// std::cout << "error >> unhandled_exception" << std::endl;
	printf ("error >> unhandled exception\n");
	exit (-1);
}