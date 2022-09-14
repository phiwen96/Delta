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
#include <utility>

// import Coro;
// import Graphics;
// import <iostream>;

auto current_file_path () noexcept -> std::string {
	return std::filesystem::current_path().string() + '/' + std::source_location::current().file_name();
}

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

template <typename...>
struct future;

template <>
struct future <> {
	struct promise_type {
		auto get_return_object () {
			return future {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> std::suspend_never {
			return {};
		}
		void unhandled_exception() {std::cout << "error >> unhandled_exception" << std::endl; exit (-1);}
		auto return_void () -> void {}
	};
	explicit future (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future (future&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future (future const&) = delete;
	~future () {if (handle) handle.destroy();}
	std::coroutine_handle <promise_type> handle;
};

struct [[nodiscard]] future_read {
	struct promise_type {
		std::coroutine_handle <> continuation;
		char* txt;
		auto get_return_object () {
			return future_read {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> auto {
			std::cout << "final_suspend" << std::endl;
			struct awaitable {
				auto await_ready () noexcept -> bool {
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					// return h.promise().continuation;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> void {
					std::cout << "final_suspend::await_resume" << std::endl;
				}
			};
			return awaitable {};
		}
		void unhandled_exception() {std::cout << "error >> unhandled_exception" << std::endl; exit (-1);}
		auto return_value (char* t) -> void {
			txt = t;
		}
		auto await_transform (struct aiocb& a) noexcept -> decltype (auto) {
			struct awaitable {
				auto await_ready () noexcept -> bool {
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> void {
					std::cout << "await_suspend" << std::endl;
				}
				auto await_resume () noexcept -> void {
					std::cout << "await_resume" << std::endl;
				}
			};
			// a.aio_sigevent.sigev_value.sival_ptr = & my_aiocb;
			a.aio_sigevent.sigev_value.sival_ptr = std::coroutine_handle <promise_type>::from_promise (*this).address ();

			if (auto res = aio_read (&a); res == -1) {
				perror ("aio_read");
				exit (-1);
			}

			return awaitable {};
		}
	};
	auto get () noexcept -> char* {
		return handle.promise().txt;
	}
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> c) noexcept -> std::coroutine_handle <promise_type> {
		std::cout << "await_suspend" << std::endl;
		handle.promise().continuation = c;
		return handle;
	}
	auto await_resume () noexcept -> char* {
		std::cout << "await_resume" << std::endl;
		return handle.promise().txt;
	}
	explicit future_read (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future_read (future_read&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future_read (future_read const&) = delete;
	~future_read () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

// auto spawn_future () -> future {
// 	// co_await ()
// 	co_return;
// }

void aio_completion_handler (sigval sigval) {
	auto coro_handle = std::coroutine_handle <future_read::promise_type>::from_address (sigval.sival_ptr);
	coro_handle.resume();
	// auto my_aiocb = (struct aiocb *)sigval.sival_ptr;
	// std::cout << std::this_thread::get_id() << std::endl;
	// std::cout << (char const*) my_aiocb->aio_buf << std::endl;
	// if (aio_error (my_aiocb) == -1) {
	// 	perror ("aio_error");
	// 	exit (-1);
	// }
}

auto async_read (char const* path) -> future_read {
	auto fd = open (path, O_RDONLY);
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
	
	struct aiocb my_aiocb {};

	my_aiocb.aio_fildes = fd;//fileno (fp),
	my_aiocb.aio_buf = malloc (filesize * sizeof (char));
	my_aiocb.aio_nbytes = (size_t) filesize;
	my_aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
	my_aiocb.aio_sigevent.sigev_notify_function = aio_completion_handler;
	my_aiocb.aio_sigevent.sigev_notify_attributes = NULL;
	std::cout << "co_await my_aiocb" << std::endl;
	co_await my_aiocb;
	std::cout << "co_await my_aiocb" << std::endl;
	if (aio_error (&my_aiocb) == -1) {
		perror ("aio_error");
		exit (-1);
	}

	std::cout << "yay" << std::endl;

	co_return (char*) my_aiocb.aio_buf;
}

auto do_some_work () -> future <> {
	std::cout << "doing some work..." << std::endl;
	auto txt = co_await async_read (current_file_path().c_str());
	std::cout << "oui" << std::endl;
	std::cout << txt << std::endl;
	co_return;
}




auto main (int argc, char** argv) -> int {
	// auto f = spawn_future ();
	// return 0;
	// std::cout << std::filesystem::current_path().string() + '/' + std::source_location::current().file_name() << std::endl;
	
	// auto future_txt = async_read (current_file_path().c_str());
	// auto dd = do_some_work ();
	async_read (current_file_path().c_str());
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
	
	// auto current_file = current_file_path ();


	// auto fd = open (current_file.c_str(), O_RDONLY);
	// if (fd == -1) {
	// 	perror ("open");
	// 	exit(-1);
	// }
	// struct stat st;
	// if (fstat (fd, &st) == -1) {
	// 	perror ("fstat");
	// 	exit (-1);
	// }
	// long long filesize = st.st_size;
	
	// struct aiocb my_aiocb {
		
		
	// 	// .aio_lio_opcode = LIO_READ
	// };

	// my_aiocb.aio_fildes = fd;//fileno (fp),
	// 	my_aiocb.aio_buf = malloc (filesize * sizeof (char));
	// 	my_aiocb.aio_nbytes = (size_t) filesize;

	// my_aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
	// 		my_aiocb.aio_sigevent.sigev_notify_function = aio_completion_handler;
	// 		my_aiocb.aio_sigevent.sigev_notify_attributes = NULL;
	// 		my_aiocb.aio_sigevent.sigev_value.sival_ptr = & my_aiocb;

	//  if (auto res = aio_read(&my_aiocb); res == -1) {
	// 	perror ("aio_read");
	// 	if (res == ECANCELED) {
	// 		std::cout << "EAGAIN" << std::endl;
	// 	}

	// 	exit (-1);
	// }

	// std::cout << std::this_thread::get_id() << std::endl;
	


	return 0;
}