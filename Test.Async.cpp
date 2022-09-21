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

struct io_data {

};

auto main (int argc, char** argv) -> int {
	auto fd = open (filepath, O_RDONLY);
	auto size = get_file_size (fd);
	auto io = iovec {.iov_base = malloc (size), .iov_len = size};
	// if (readv (fd, &io, 1) == -1) {
	// 	perror ("readv");
	// 	exit (-1);
	// }
	// std::cout << (char const*) io.iov_base << std::endl;

	// auto info = thread_info {.ring = {}, .efd = eventfd (0, 0)};

	struct io_uring ring;

	if (io_uring_queue_init (10, &ring, 0) == -1) {
		perror ("io_uring_queue_init");
		exit (-1);
	}
	// io_uring_register_eventfd(&info.ring, info.efd);

	struct io_uring_sqe * sqe;

	// if (not (sqe = io_uring_get_sqe (&info.ring))) {
	// 	perror ("io_uring_get_sqe");
	// 	exit (-1);
	// }
	// io_uring_prep_readv (sqe, fd, &io, 1, 0);
	// io_uring_sqe_set_data (sqe, &io);
	// io_uring_submit (&info.ring);

	if (not (sqe = io_uring_get_sqe (&ring))) {
		perror ("io_uring_get_sqe");
		exit (-1);
	}
	io_uring_prep_readv (sqe, STDIN_FILENO, &io, 1, 0);
	io_uring_sqe_set_data (sqe, &io);
	io_uring_submit (&ring);
	

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
			io_uring_cqe_seen (&ring, cqe);
			std::cout << "yay " << (char const*) ((struct iovec*) cqe -> user_data)->iov_base << std::endl;

			if (not (sqe = io_uring_get_sqe (&ring))) {
				perror ("io_uring_get_sqe");
				exit (-1);
			}
			
			io_uring_prep_writev (sqe, STDOUT_FILENO, &io, 1, 0);
			io_uring_sqe_set_data (sqe, &io);
			io_uring_submit (&ring);
		}
	}
	

	io_uring_queue_exit (&ring);

	// io_uring_enter ();
	return 0;
}