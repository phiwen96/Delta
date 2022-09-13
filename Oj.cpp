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

void async_signal_handler (int signo, siginfo_t *info, void *context) {  
	auto ptr =(struct aiocb *)(info->si_value.sival_ptr); 

	// std::cout << ((char const*) ptr->aio_buf)[0] << std::endl;
	
	if (auto err = aio_error (ptr); err == -1) {

		perror ("error >> aio_error");
		switch (err)
		{
			case EINPROGRESS:
				std::cout << "error >> aio_error (EINPROGRESS)" << std::endl;
				break;
			
			case ECANCELED:
				std::cout << "error >> aio_error (ECANCELED)" << std::endl;
				break;

			case EINVAL:
				std::cout << "error >> aio_error (EINVAL)" << std::endl;
				break;

			case ENOSYS:
				std::cout << "error >> aio_error (ENOSYS)" << std::endl;
				break;

			case EOVERFLOW:
				std::cout << "error >> aio_error (EOVERFLOW)" << std::endl;
				break;
		
			default:
				std::cout << "error >> aio_error (unknown)" << std::endl;
				break;
		}
		
		exit (-1);
	}

	auto txt = (const char*) ptr->aio_buf;
	std::cout << txt << std::endl;
	// printf("read=%s", (char *)ptr->aio_buf);  
	// std::cout << "yo" << std::endl;
}  

void aio_completion_handler(sigval sigval) {
	auto my_aiocb = (struct aiocb *)sigval.sival_ptr;
	std::cout << (char const*) my_aiocb->aio_buf << std::endl;
	// if (aio_error (my_aiocb) == -1) {
	// 	perror ("aio_error");
	// 	exit (-1);
	// }
}

auto main (int argc, char** argv) -> int {
	// auto f = spawn_future ();
	// return 0;
	std::cout << std::filesystem::current_path().string() + '/' + std::source_location::current().file_name() << std::endl;
	auto current_file = std::filesystem::current_path().string() + '/' + std::source_location::current().file_name();
	// auto * fp = fopen (current_file.c_str(), "r");
	// if (fp == nullptr) {
	// 	std::cout << "error" << std::endl;
	// 	exit (-1);
	// }
	// if (fseek(fp, 0, SEEK_END) == -1) {
	// 	std::cout << "error" << std::endl;
	// 	exit (-1);
	// }
	// auto filelen = ftell(fp);
	// rewind(fp);
	
	// malloc(sizeof(char) * (bufsize + 1));
	// auto buffer = new char [10000];
	// auto * buffer = malloc (20);
	// if (buffer == NULL) {
	// 	perror ("malloc");
	// 	exit (-1);
	// }
	// std::cout << filelen << std::endl;
	// size_t newLen = fread (buffer, sizeof(char), filelen, fp);
	// std::cout << buffer << std::endl;
	// std::cout << newLen << std::endl;
	
	struct sigaction siga {};

	
	
	siga.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&siga.sa_mask);  
	siga.sa_sigaction = async_signal_handler;
	
	if (sigaction(SIGUSR1, &siga, nullptr) == -1) {
		std::cout << "error" << std::endl;
		exit (-1);
	}

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
	
	// int flags = fcntl(fd, F_GETFL, 0);
    // if (flags == -1) {
	// 	perror ("fcntl");
	// 	exit (-1);
	// }
    // int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	auto my_aiocb = (struct aiocb*) malloc (sizeof (struct aiocb));
	
	my_aiocb->aio_fildes = fd;
	my_aiocb->aio_offset = 0;
	my_aiocb->aio_buf = malloc (filesize * sizeof (char));
	my_aiocb->aio_nbytes = filesize;
	my_aiocb->aio_reqprio = 0;
	// my_aiocb->aio_sigevent->sigev_value->sival_int = 0;
	// my_aiocb->aio_sigevent->sigev_notify_function
	my_aiocb->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	my_aiocb->aio_sigevent.sigev_signo = SIGUSR1;
	// my_aiocb->aio_sigevent->sigev_notify_attributes = NULL;
	my_aiocb->aio_sigevent.sigev_value.sival_ptr = my_aiocb;
	// my_aiocb.aio_sigevent.sigev_value = sigval {
	// 	.sival_ptr = & my_aiocb,
	// };
	// fclose (fp);
	// struct aiocb aiocb {
	// 	.aio_fildes = fileno (fp),
	// 	.aio_buf = buffer,
	// 	.aio_nbytes = (size_t) filelen,
	// 	.aio_sigevent = sigevent {
	// 		.sigev_notify = SIGEV_THREAD,
	// 		.sigev_notify_function = async_signal_handler,
	// 		.sigev_notify_attributes = NULL
	// 	},
	// 	.aio_lio_opcode = LIO_READ
	// };

	// aiocb.aio_sigevent.sigev_value = sigval {
	// 	.sival_ptr = & aiocb
	// };

	 if (auto res = aio_read(my_aiocb); res == -1) {
		perror ("aio_read");
		if (res == ECANCELED) {
			std::cout << "EAGAIN" << std::endl;
		}

		exit (-1);
	 }
	
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