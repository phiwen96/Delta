export module App:Type.Interface;

import Concepts.Bool;
import Window;

// export template <typename T>
// concept App = requires (T app) {
// 	{T {"title"}} noexcept;
// 	{app.run()} noexcept -> Bool;
// 	{app.new_window ()} noexcept -> Window;
// };

export struct app {
	app (auto&& title) noexcept;
	auto run () noexcept -> bool;
	auto new_window () noexcept -> window; 
};

