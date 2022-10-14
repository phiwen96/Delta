export module Coro:Concept;

import Promise;

export template <typename T>
concept Coro = requires (T coro) {
	requires Promise <typename T::promise_type>;
};