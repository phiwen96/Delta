export module App:Concept;

import Coro;

auto run_app (auto & app) noexcept -> bool {
	return true;
}


export template <typename T>
concept App = // Coro <T> and 
requires (T app) {
	true;
};