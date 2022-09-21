import Async;
#if not defined (WINDOWS)
// #include <linux/io_uring.h>
#include <liburing.h>
#endif
#include <iostream>
// #include <sys/uio.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <stdio.h>
#include <semaphore>
#include <thread>
#include <coroutine>
#include <utility>
// #include <stdlib.h>
/*
	copy stdin to stdout. Using shell redirection, you should be able to copy files with this example

	STDIN_FILENO
	STDOUT_FILENO
*/

constexpr auto filepath = "/home/ph/Coding/Delta/Test.Async.cpp";

auto get_file_size (int fd) noexcept -> size_t {
	struct stat r {};
	if (fstat (fd, &r) == -1) {
		perror ("fstat");
		exit (-1);
	}
	return r.st_size;
} 


struct thread_info {
	struct io_uring ring;
	int efd;
};



struct io_data {

};

auto sem = std::binary_semaphore {0};

struct my_coro_handle {
	auto await_ready () noexcept {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> h) noexcept {
		// return h;
		handle = h;
		return false;
		
	}
	auto await_resume () noexcept {
		return handle;
	}
	std::coroutine_handle <> handle;
};

template <typename...>
struct future;

template <>
struct future <> {
	struct promise_type {
		std::coroutine_handle <> continuation;
		auto get_return_object () -> future {
			return future {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> auto {
			struct awaitable {
				auto await_ready () noexcept -> bool {
					
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					// return h.promise().continuation;
					std::cout << "future::final_suspend::await_suspend" << std::endl;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> void {
					std::cout << "future::final_suspend::await_resume" << std::endl;
					// std::cout << "final_suspend::await_resume" << std::endl;
				}
			};
			return awaitable {};
		}
		void unhandled_exception() {std::cout << "error >> unhandled_exception" << std::endl; exit (-1);}
		auto return_void () -> void {}
	};
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> c, auto from = __builtin_FUNCTION()) noexcept -> std::coroutine_handle <promise_type> {
		std::cout << "future::await_suspend called from " << from << std::endl;
		// std::cout << "await_suspend" << std::endl;
		handle.promise().continuation = c;
		return handle;
	}
	auto await_resume () noexcept -> void {
		std::cout << "future::await_resume" << std::endl;
	}
	explicit future (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future (future&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future (future const&) = delete;
	~future () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

struct /*[[nodiscard]]*/ future_io {
	struct promise_type {
		std::coroutine_handle <> continuation;
		char* txt;
		auto get_return_object () -> future_io {
			return future_io {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> auto {
			// std::cout << "final_suspend" << std::endl;
			struct awaitable {
				auto await_ready () noexcept -> bool {
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					std::cout << "future_io::final_suspend::await_suspend" << std::endl;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> void {
					std::cout << "future_io::final_suspend::await_resume" << std::endl;
				}
			};
			return awaitable {};
			// return std::suspend_never {};
			// return std::suspend_always {};
		}
		void unhandled_exception() {std::cout << "error >> unhandled_exception" << std::endl; exit (-1);}
		auto return_value (char* t) -> void {
			txt = t;
		}
		// auto await_transform (struct aiocb& a) noexcept -> auto {
		// 	struct awaitable {
		// 		auto await_ready () noexcept -> bool {
		// 			return false;
		// 		}
		// 		auto await_suspend (std::coroutine_handle <> h, char const* from = __builtin_FUNCTION(), int line = __builtin_LINE()) noexcept -> void {
		// 			std::cout << "future_io::await_transform::await_suspend called from " << from << line << std::endl;
		// 			// std::cout << "await_suspend" << std::endl;
		// 		}
		// 		auto await_resume () noexcept -> void {
		// 			std::cout << "future_io::await_transform::await_resume" << std::endl;
		// 		}
		// 	};
		// 	// a.aio_sigevent.sigev_value.sival_ptr = & my_aiocb;
		// 	a.aio_sigevent.sigev_value.sival_ptr = std::coroutine_handle <promise_type>::from_promise (*this).address ();

		// 	if (auto res = aio_read (&a); res == -1) {
		// 		perror ("aio_read");
		// 		exit (-1);
		// 	}

		// 	return awaitable {};
		// }
	};
	auto get () noexcept -> char* {
		return handle.promise().txt;
	}
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> c, char const* from = __builtin_FUNCTION(), int line = __builtin_LINE()) noexcept//std::coroutine_handle <promise_type> 
	{
		std::cout << "future_io::await_suspend called from " << from << line << std::endl;
		// std::cout << "await_suspend" << std::endl;
		handle.promise().continuation = c;
		// return handle;
	}
	auto await_resume () noexcept -> char* {
		std::cout << "future_io::await_resume" << std::endl;
		return handle.promise().txt;
	}
	explicit future_io (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future_io (future_io&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future_io (future_io const&) = delete;
	~future_io () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

struct io_uring ring;

auto async_read (char const* path, char const* txt) noexcept -> future_io {
	auto fd = open (filepath, O_RDONLY);
	if (fd == -1) {
		perror ("open");
		exit (-1);
	}
	auto size = get_file_size (fd);
	struct iovec io {.iov_base = malloc (size), .iov_len = size};
	// auto io = (iovec*) malloc (sizeof (iovec));
	// io -> iov_base = malloc (size);
	// io -> iov_len = size;
	struct io_uring_sqe * sqe;
	if (not (sqe = io_uring_get_sqe (&ring))) {
		perror ("io_uring_get_sqe");
		exit (-1);
	}
	io_uring_prep_readv (sqe, fd, &io, 1, 0);
	auto h = co_await my_coro_handle {};
	io_uring_sqe_set_data (sqe, h.address ());
	io_uring_submit (&ring);
	co_await std::suspend_always {};
	if (close (fd) == -1) {
		perror ("close");
		exit (-1);
	}
	co_return (char*) io.iov_base;
}

auto listener_thread (void * data) noexcept -> void * {
	auto * info = (thread_info *) data;
	auto v = eventfd_t {};
	if (eventfd_read (info -> efd, &v) == -1) {
		perror ("eventfd_read");
		exit (-1);
	}
	struct io_uring_cqe * cqe;
	if (io_uring_wait_cqe (&info -> ring, &cqe) == -1) {
		perror ("io_uring_wait_cqe");
		exit (-1);
	}
	if (cqe -> res == -1) {
		perror ("async error");
		exit (-1);
	}
	io_uring_cqe_seen (&info -> ring, cqe);

	std::cout << (char const*) ((struct iovec*) cqe -> user_data)->iov_base << std::endl;
	return nullptr;
}

auto main (int argc, char** argv) -> int {

	// std::thread{[]{
	// 	sleep (10);
	// 	sem.release ();
	// }}.detach();
	// sem.acquire ();

	// return 0;

	// auto fd = open (filepath, O_RDONLY);
	// auto size = get_file_size (fd);
	// auto io = iovec {.iov_base = malloc (size), .iov_len = size};
	// if (readv (fd, &io, 1) == -1) {
	// 	perror ("readv");
	// 	exit (-1);
	// }
	// std::cout << (char const*) io.iov_base << std::endl;

	// auto info = thread_info {.ring = {}, .efd = eventfd (0, 0)};

	

	if (io_uring_queue_init (10, &ring, 0) == -1) {
		perror ("io_uring_queue_init");
		exit (-1);
	}
	// io_uring_register_eventfd(&info.ring, info.efd);

	// struct io_uring_sqe * sqe;

	// if (not (sqe = io_uring_get_sqe (&info.ring))) {
	// 	perror ("io_uring_get_sqe");
	// 	exit (-1);
	// }
	// io_uring_prep_readv (sqe, fd, &io, 1, 0);
	// io_uring_sqe_set_data (sqe, &io);
	// io_uring_submit (&info.ring);

	// if (not (sqe = io_uring_get_sqe (&ring))) {
	// 	perror ("io_uring_get_sqe");
	// 	exit (-1);
	// }
	// io_uring_prep_readv (sqe, STDIN_FILENO, &io, 1, 0);
	// io_uring_sqe_set_data (sqe, &io);
	// io_uring_submit (&ring);
	

	// auto t = pthread_t {};
	// pthread_create (&t, nullptr, listener_thread, (void*) &info);
	// sleep (2);
	// pthread_join (t, nullptr);

	struct io_uring_cqe* cqe;
	while (true) {
		if (io_uring_peek_cqe (&ring, &cqe) == 0) {
			if (cqe -> res == -1) {
				perror ("async error");
				exit (-1);
			}
			std::coroutine_handle <future_io::promise_type>::from_address ((void*) cqe -> user_data).resume ();
			io_uring_cqe_seen (&ring, cqe);
			// std::cout << "yay " << (char const*) ((struct iovec*) cqe -> user_data)->iov_base << std::endl;
			// struct io_uring_sqe* sqe;

			// if (not (sqe = io_uring_get_sqe (&ring))) {
			// 	perror ("io_uring_get_sqe");
			// 	exit (-1);
			// }

			// io_uring_prep_writev (sqe, STDOUT_FILENO, &io, 1, 0);
			// io_uring_sqe_set_data (sqe, &io);
			// io_uring_submit (&ring);
		}
	}
	

	io_uring_queue_exit (&ring);

	// io_uring_enter ();
	return 0;
}