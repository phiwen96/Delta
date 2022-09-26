export module App:Interface;

import Concepts.Bool;
import Window;

export template <typename T>
concept App = requires (T app) {
	{app.run()} noexcept -> Bool;
};

export struct app {
	app (auto&& title) noexcept;
	auto run () noexcept -> bool;
};

