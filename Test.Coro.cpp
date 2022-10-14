#include <iostream>
#include <semaphore>
#include <coroutine>
#include <utility>
#include <stdio.h>
import Coro;

// using coro_t = coro <>





auto app () noexcept -> coro <std::suspend_never> {
	std::cout << "yoyoyo" << std::endl;
	co_return;
}


auto main (int, char**) -> int {
	Coro auto c = app ();

	printf ("hey\n");
	return 0;
}