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
// #include <stdlib.h>

constexpr auto filepath = "/home/ph/Coding/Delta/Test.Async.cpp";

auto get_file_size (int fd) noexcept -> size_t {
	struct stat r {};
	if (fstat (fd, &r) == -1) {
		perror ("fstat");
		exit (-1);
	}
	return r.st_size;
} 

struct io_uring ring;

struct thread_info {
	struct io_uring ring;
	int efd;
};

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
	auto fd = open (filepath, O_RDONLY);
	auto size = get_file_size (fd);
	auto io = iovec {.iov_base = malloc (size), .iov_len = size};
	// if (readv (fd, &io, 1) == -1) {
	// 	perror ("readv");
	// 	exit (-1);
	// }
	// std::cout << (char const*) io.iov_base << std::endl;

	auto info = thread_info {.ring = {}, .efd = eventfd (0, 0)};

	if (io_uring_queue_init (1, &info.ring, 0) == -1) {
		perror ("io_uring_queue_init");
		exit (-1);
	}
	io_uring_register_eventfd(&info.ring, info.efd);

	auto * sqe = io_uring_get_sqe (&info.ring);
	if (not sqe) {
		perror ("io_uring_get_sqe");
		exit (-1);
	}
	io_uring_prep_readv (sqe, fd, &io, 1, 0); 
	io_uring_sqe_set_data (sqe, &io);
	io_uring_submit (&info.ring);
	auto t = pthread_t {};
	pthread_create (&t, nullptr, listener_thread, (void*) &info);
	// sleep (2);
	pthread_join (t, nullptr);
	io_uring_queue_exit (&info.ring);

	// io_uring_enter ();
	return 0;
}