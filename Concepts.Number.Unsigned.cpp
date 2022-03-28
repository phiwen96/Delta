export module Delta.Concepts.Number.Unsigned;

import Delta.Concepts.Convertible;

export template <typename T>
concept Unsigned = 
	Convertible <T, unsigned short> or 
	Convertible <T, unsigned short int> or 
	Convertible <T, unsigned> or 
	Convertible <T, unsigned int> or 
	Convertible <T, unsigned long> or 
	Convertible <T, unsigned long int> or 
	Convertible <T, unsigned long long> or 
	Convertible <T, unsigned long long int>;