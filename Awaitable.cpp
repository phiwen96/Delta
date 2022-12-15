module;
#include <coroutine>
export module Awaitable;

import Concepts.Bool;

export template <typename T>
concept Awaitable = requires (T t, std::coroutine_handle <> waiting_coro) {
	{t.await_ready()} noexcept -> Bool;// bool
	{t.await_suspend (waiting_coro)} noexcept; // void, std::coroutine_handle <> which to transfer control to
	{t.await_resume()} noexcept; // void, any
};


