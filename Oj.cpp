// import Coro;
// import A; 
// import Coro.Promise;
// #include <iostream>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <coroutine>
#include <iostream>
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <source_location>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <thread>
// import Coro;
// import Graphics;
// import <iostream>;

template <typename... T>
struct overload : T... {
	overload (T... t) noexcept : T {t}... {}
};

struct Window {
	GLFWwindow * handle;
	struct {
		void (*mouse_button) (GLFWwindow* window, int button, int action, int mods) noexcept = nullptr;
	} callbacks;

	Window (int width, int height) {
		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
		handle = glfwCreateWindow (width, height, "", nullptr, nullptr);
	}

	auto register_callbacks () {
		if (callbacks.mouse_button != nullptr) {
			glfwSetMouseButtonCallback (handle, callbacks.mouse_button);
		}
	}

	auto set_user_pointer (auto * p) noexcept -> Window & {
		glfwSetWindowUserPointer (handle, p);
		return *this;
	}
};

struct async_read_file_t {

};

// auto async_read_file (char const* path) -> coro {
// 	co_await async_read_file (path);
	
// }




// auto make_coro () noexcept -> coro {
// 	// std::cout << "yo" << std::endl;
// 	// co_await 
// 	co_return;
// }

struct future {
	struct promise_type {
		auto get_return_object () {
			return future {std::coroutine_handle<promise_type>::from_promise (*this)};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> std::suspend_never {
			return {};
		}
		void unhandled_exception() {}
		auto return_void () -> void {}
	};
	std::coroutine_handle <promise_type> handle;
};

auto spawn_future () -> future {
	// co_await ()
	co_return;
}


void aio_completion_handler (sigval sigval) {
	auto my_aiocb = (struct aiocb *)sigval.sival_ptr;
	std::cout << std::this_thread::get_id() << std::endl;
	// std::cout << (char const*) my_aiocb->aio_buf << std::endl;
	if (aio_error (my_aiocb) == -1) {
		perror ("aio_error");
		exit (-1);
	}
}

auto main (int argc, char** argv) -> int {
	// auto f = spawn_future ();
	// return 0;
	std::cout << std::filesystem::current_path().string() + '/' + std::source_location::current().file_name() << std::endl;
	auto current_file = std::filesystem::current_path().string() + '/' + std::source_location::current().file_name();


	auto fd = open (current_file.c_str(), O_RDONLY);
	if (fd == -1) {
		perror ("open");
		exit(-1);
	}
	struct stat st;
	if (fstat (fd, &st) == -1) {
		perror ("fstat");
		exit (-1);
	}
	long long filesize = st.st_size;
	
	struct aiocb my_aiocb {
		
		
		// .aio_lio_opcode = LIO_READ
	};

	my_aiocb.aio_fildes = fd;//fileno (fp),
		my_aiocb.aio_buf = malloc (filesize * sizeof (char));
		my_aiocb.aio_nbytes = (size_t) filesize;

	my_aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
			my_aiocb.aio_sigevent.sigev_notify_function = aio_completion_handler;
			my_aiocb.aio_sigevent.sigev_notify_attributes = NULL;
			my_aiocb.aio_sigevent.sigev_value.sival_ptr = & my_aiocb;

	 if (auto res = aio_read(&my_aiocb); res == -1) {
		perror ("aio_read");
		if (res == ECANCELED) {
			std::cout << "EAGAIN" << std::endl;
		}

		exit (-1);
	 }

	std::cout << std::this_thread::get_id() << std::endl;
	
	// while (true)
	// {
	// 	/* code */
	// }
	


	// return 0;

	if (!glfwInit()) return -1;
	auto window = Window {640, 480};

	auto inputEventHandler = overload {
		[&] (auto button, auto action, auto mods) {
			std::cout << "click" << std::endl;
		} 
	};

	{
		using EventHandler = decltype (inputEventHandler) *;

		window.callbacks.mouse_button = [] (GLFWwindow* window, int button, int action, int mods) noexcept -> void {
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (button, action, mods);
		};

		window.set_user_pointer (&inputEventHandler).register_callbacks ();
	}

	

	// auto f = make_coro ();

	while (not glfwWindowShouldClose (window.handle))
	{
		glfwPollEvents ();
		/* code */
	}
	
	// f.resume ();
	// auto s = shelly {};
	// auto s = test ();
	// s.kissar ();
	// auto p = philip {};
	// auto p = prom {};
	// p.hej();
	return 0;
}