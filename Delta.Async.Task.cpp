module;
#include <coroutine>
export module Delta.Async.Task;

export namespace async
{
	template <typename T>
	struct [[nodiscard]] task 
	{
		struct promise_type 
		{
			std::coroutine_handle <> continuation;
			auto get_return_object () noexcept -> auto
			{
				return task {*this};
			}
			auto return_value (T const& t) noexcept -> void
			{

			}
			auto unhandled_exception () noexcept -> void 
			{

			}
			auto yield_value (auto&&) noexcept -> void 
			{
				
			}
			auto initial_suspend () noexcept -> auto 
			{
				return std::suspend_never {};
			}
			auto final_suspend () noexcept -> auto 
			{
				struct awaitable 
				{
					auto await_ready () noexcept -> bool 
					{
						return false;
					}
					auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> auto
					{
						return h.promise().continutaion;
					}
					auto await_resume () noexcept -> void 
					{

					}
				};

				return awaitable {};
			}
		};

		task (task&& other) noexcept : handle {(std::coroutine_handle <promise_type>&&) other.handle}
		{

		}

		~task ()
		{
			if (handle)
			{
				handle.destroy ();
			}
		}

		auto await_ready () noexcept -> bool 
		{
			return false;
		}

		auto await_suspend (std::coroutine_handle <> c) noexcept -> std::coroutine_handle <promise_type>
		{	
			handle.promise().continuation = c;
			return handle;
		}

		auto await_resume () noexcept -> void 
		{

		}

	private:
		explicit task (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)}
		{
			
		}
		std::coroutine_handle <promise_type> handle;
	};




};