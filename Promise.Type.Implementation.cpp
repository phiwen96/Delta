module;
#include <coroutine>
module Promise:Type.Implementation;

import :Type.Interface;

template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
auto promise <return_object, initial_awaiter, final_awaiter>::get_return_object () noexcept -> return_object{
	return return_object {std::coroutine_handle <promise>::from_promise (*this)};
}

template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
auto promise <return_object, initial_awaiter, final_awaiter>::initial_suspend () noexcept -> initial_awaiter{
	return {};
}

template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
auto promise <return_object, initial_awaiter, final_awaiter>::final_suspend () noexcept -> final_awaiter{
	return {};
}

template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
auto promise <return_object, initial_awaiter, final_awaiter>::return_void () noexcept -> void {

}

template <
	typename return_object, 
	typename initial_awaiter,
	typename final_awaiter>
auto promise <return_object, initial_awaiter, final_awaiter>::unhandled_exception () noexcept -> void{
	
}
