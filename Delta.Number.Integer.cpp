export module Delta:Number.Integer;

import :Convertible;

export template <typename T>
concept Integer = 
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
	Convertible <T, signed long long int> or 
	Convertible <T, unsigned short> or 
	Convertible <T, unsigned short int> or 
	Convertible <T, unsigned> or 
	Convertible <T, unsigned int> or 
	Convertible <T, unsigned long> or 
	Convertible <T, unsigned long int> or 
	Convertible <T, unsigned long long> or
	Convertible <T, unsigned long long int>;