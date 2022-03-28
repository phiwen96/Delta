export module Delta.Concepts.Number.Signed;

import Delta.Concepts.Convertible;

export template <typename T>
concept Signed =
	Convertible <T, short> or 
	Convertible <T, short int> or 
	Convertible <T, signed short> or 
	Convertible <T, signed short int> or 
	Convertible <T, int> or 
	Convertible <T, signed> or 
	Convertible <T, signed int> or 
	Convertible <T, long> or 
	Convertible <T, long int> or 
	Convertible <T, signed long> or 
	Convertible <T, signed long int> or 
	Convertible <T, long long> or 
	Convertible <T, long long int> or 
	Convertible <T, signed long long> or 
	Convertible <T, signed long long int>;