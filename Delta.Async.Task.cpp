module;
#include <coroutine>
export module Delta.Async.Task;

export namespace async
{
	template <typename T>
	struct task 
	{
		struct promise_type 
		{
			std::coroutine_handle <> continuation;
			auto get_return_object () noexcept -> auto
			{
				return task {*this};
			}
			auto return_void () noexcept -> void
			{

			}
			auto unhandled_exception () noexcept -> void 
			{

			}
			auto initial_suspend () noexcept -> auto 
			{
				return std::suspend_never {};
			}
			auto final_suspend () noexcept ->
		};

		constexpr task (task&& other) noexcept : handle {(std::coroutine_handle <promise_type>&&) other.handle}
		{

		}

		~task ()
		{
			if (handle)
			{
				handle.destroy;
			}
		}

		auto await_ready () noexcept -> bool 
		{
			return false;
		}

		auto await_suspend (std::coroutine_handle <> c)  -> auto
		{	

		}

	private:
		constexpr explicit task (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)}
		{
			
		}
		std::coroutine_handle <promise_type> handle;
	};




};