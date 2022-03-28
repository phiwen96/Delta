export module Delta.Concepts.Class;

template <class T> char test(int T::*);   //this line
struct two
{
	char c[2];
};

template <class T> two test(...);    

export template <typename T>
concept Class = sizeof(test<T>(0))==1;