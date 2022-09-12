export module Coro.Awaitable;

export template <typename T>
concept Awaitable = requires (T t, std::coroutine_handle <> waiting_coro) {
	t.await_ready(); // bool
	t.await_suspend(waiting_coro); // void, std::coroutine_handle <> which to transfer control to
	t.await_resume(); // void, any
};
