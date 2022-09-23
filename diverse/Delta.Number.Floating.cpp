export module Delta:Number.Floating;

import :Convertible;

export template <typename T>
concept Floating = 
	Convertible <T, float> or 
	Convertible <T, double> or 
	Convertible <T, long double>;