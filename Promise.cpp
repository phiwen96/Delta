export module Promise;

export import :Type;

// import <coroutine>;

// export template <typename T>
// concept Promise = requires (T t) {
// 	t.get_return_object();
// 	t.initial_suspend();
// 	t.final_suspend();
// 	t.return_void();
// 	t.unhandled_exception();
// };

// export template <
// 	typename return_object, 
// 	typename initial_awaiter,
// 	typename final_awaiter>
// struct promise;

// export template <
// 	typename return_object, 
// 	typename initial_awaiter,
// 	typename final_awaiter>
// struct promise {
// 	auto get_return_object () noexcept -> return_object{
// 	return return_object {std::coroutine_handle <promise>::from_promise (*this)};
// }

// 	auto initial_suspend () noexcept -> initial_awaiter{
// 	return {};
// 	}
// 	auto final_suspend () noexcept -> final_awaiter{
// 	return {};
// }
// 	auto return_void () noexcept -> void;
// 	auto unhandled_exception () noexcept -> void{
	
// }
// };

// template <
// 	typename return_object, 
// 	typename initial_awaiter,
// 	typename final_awaiter>
// auto promise <return_object, initial_awaiter, final_awaiter>::return_void () noexcept -> void {
	
// }
