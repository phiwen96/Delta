export module Delta.Concepts.Bool;

import Delta.Concepts.Convertible;

export template <typename T>
concept Bool = Convertible <T, bool>;