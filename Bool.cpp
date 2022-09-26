export module Concepts.Bool;

export template <typename T> 
concept Bool = requires (T t, bool b) {
	b = t;
};