import Async;
#if not defined (WINDOWS)
// #include <linux/io_uring.h>
#include <liburing.h>
#endif
#include <iostream>
// #include <sys/uio.h>
#include <sys/stat.h>
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

auto main (int argc, char** argv) -> int {
	auto fd = open (filepath, O_RDONLY);
	auto size = get_file_size (fd);
	auto io = iovec {.iov_base = malloc (size), .iov_len = size};
	// if (readv (fd, &io, 1) == -1) {
	// 	perror ("readv");
	// 	exit (-1);
	// }
	// std::cout << (char const*) io.iov_base << std::endl;

	auto ring = io_uring {};

	if (io_uring_queue_init (1, &ring, 0) == -1) {
		perror ("io_uring_queue_init");
		exit (-1);
	}

	auto * sqe = io_uring_get_sqe (&ring);
	io_uring_prep_readv (sqe, fd, &io, 1, 0); 
	io_uring_sqe_set_data (sqe, &io);
	io_uring_submit (&ring);
	io_uring_queue_exit (&ring);

	// io_uring_enter ();
	return 0;
}