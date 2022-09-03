module Coro;


import <iostream>;
import <coroutine>;
import <utility>;

import :Promise;


struct coro {

	coro (coro && other) noexcept : handle {std::exchange (other.handle, {})} {
		std::cout << __FUNCTION__ << std::endl;
	}
	~coro () {
		if (handle) {
			handle.destroy ();
		}
	}

	struct promise_type {
		auto get_return_object (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> coro {
			std::cout << "\"" << __LINE__ << " " << "coro::promise_type::get_return_object ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			return coro {std::coroutine_handle <promise_type>::from_promise (*this), "get_return_object ()"};
		}
		auto initial_suspend (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> std::suspend_always {
			std::cout << "\"" << __LINE__ << " " << "coro::promise_type::initial_suspend ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			return {};
		}
		auto return_void (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> void {
			std::cout << "\"" << __LINE__ << " " << "coro::promise_type::return_void ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
		}
		auto unhandled_exception (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> void {
			std::cout << "\"" << __LINE__ << " " << "coro::promise_type::unhandled_exception ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			std::terminate ();
		}

		struct final_awaiter {
			auto await_ready (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> bool {
				std::cout << "\"" << __LINE__ << " " << "coro::promise_type::final_awaiter::await_ready ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
				return false;
			}
			auto await_suspend (std::coroutine_handle <promise_type> handle, char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> std::coroutine_handle <> {
				std::cout << "\"" << __LINE__ << " " << "coro::promise_type::final_awaiter::await_suspend ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
				return handle.promise().continuation;
			}
			auto await_resume (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> void {
				std::cout << "\"" << __LINE__ << " " << "coro::promise_type::final_awaiter::await_resume ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;

			}
		};

		auto final_suspend (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> final_awaiter {
			std::cout << "\"" << __LINE__ << " " << "coro::promise_type::await_resume ()\" called from  \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			return {};
		}

		std::coroutine_handle <> continuation;
	};
	struct awaiter {
		auto await_ready (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> bool {
			std::cout << "\"" << __LINE__ << " coro::awaiter::await_ready ()\" called from << \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			return false;
		}
		auto await_suspend (std::coroutine_handle <> continuation, char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> std::coroutine_handle <> {
			std::cout << "\"" << __LINE__ << " coro::awaiter::await_suspend (std::coroutine_handle <> continuation)\" called from << \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
			handle.promise().continuation = continuation;
			return handle;
		}
		auto await_resume (char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept -> void {
			std::cout << "\"" << __LINE__ << " coro::awaiter::await_resume ()\" called from << \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
		}
		explicit awaiter (std::coroutine_handle <promise_type> handle, char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept : handle {handle} {
			std::cout << "\"" << __LINE__ << " coro::awaiter::awaiter (std::coroutine_handle <promise_type> handle)\" called from << \"" << called_from_line << " " << called_from_function << "\"" << std::endl;				
		}
		private:
		std::coroutine_handle <promise_type> handle;
	};
	auto operator co_await () && noexcept -> auto {
		std::cout << "\"" << __LINE__ << " coro::co_await ()\"" << std::endl;
		
		return awaiter {handle};
	}

private:
	explicit coro (std::coroutine_handle <promise_type> handle, char const* called_from_function = __builtin_FUNCTION (), int called_from_line = __builtin_LINE ()) noexcept : handle {handle} {
		std::cout << "\"" << __LINE__ << " coro::coro (std::coroutine_handle <promise_type> handle)\" called from << \"" << called_from_line << " " << called_from_function << "\"" << std::endl;
	}

	std::coroutine_handle <promise_type> handle;
};


