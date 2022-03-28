export module Delta.Concepts.Number.Floating;

import Delta.Concepts.Convertible;

export template <typename T>
concept Floating = 
	Convertible <T, float> or 
	Convertible <T, double> or 
	Convertible <T, long double>;