export module Delta:Bool;

import :Convertible;

export template <typename T>
concept Bool = Convertible <T, bool>;