
#include <stdio.h>	   // for fprintf()
#include <unistd.h>	   // for close(), read()
#ifdef LINUX
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <sys/inotify.h>
#endif
#include <string.h>	   // for strncmp
#include <errno.h>
#include <stdlib.h>
#include <iostream>

import Delta;
using std::endl, std::cout, std::cin, std::string;

auto input = std::string{};

auto main(int argc, char **argv) -> int
{

	// no input
	if (argc < 2)
	{

		// run with user interface

		// ask for a path
		cout << "path >> ";
		cin >> input;
		auto path = "/home/ph/Documents/hej.txt";
#ifdef LINUX
		auto inotify_fd = inotify_init1(IN_NONBLOCK);
		if (inotify_fd == -1)
		{
			perror("inotify_init1");
			exit(EXIT_FAILURE);
		}
		auto wd = inotify_add_watch(inotify_fd, path, IN_MODIFY); 
		if (wd == -1)
		{
			fprintf(stderr, "Cannot watch '%s': %s\n",
					path, strerror(errno));
			exit(EXIT_FAILURE);
		}

		auto epoll_fd = epoll_create1(0);

		if (epoll_fd == -1)
		{
			fprintf(stderr, "Failed to create epoll file descriptor\n");
			return 1;
		}

		auto event1 = epoll_event {};
		event1.events = EPOLLIN;
		// STDIN_FILENO

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event1))
		{
			fprintf(stderr, "Failed to add file descriptor to epoll\n");
			close(epoll_fd);
			return 1;
		} 
		auto event2 = epoll_event {};
		event2.events = EPOLLIN;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &event2))
		{
			fprintf(stderr, "Failed to add file descriptor to epoll\n");
			close(epoll_fd);
			return 1;
		}


		constexpr auto max_events = 20;
		epoll_event events [max_events];
		cout << "waiting for input..." << endl;
		auto event_count = epoll_wait(epoll_fd, events, max_events, 30000);
		// cout << "ready events = " << event_count << endl;
		for (auto i = 0; i < event_count; ++i)
		{
			if (events [i].events & EPOLLIN)
			{
				cout << "whaaaat" << endl;
			}
			break; 
			// printf("Reading file descriptor '%d' -- ", events[i].data.fd);
			constexpr auto read_size = 10;
			char read_buffer [read_size];
			auto bytes_read = read(events[i].data.fd, read_buffer, read_size);
			// printf("%zd bytes read.\n", bytes_read);
			read_buffer[bytes_read] = '\0';
			printf("Read '%s'\n", read_buffer);

			// if(!strncmp(read_buffer, "stop\n", 5))
			// running = 0;
		}

		if (close(epoll_fd))
		{
			fprintf(stderr, "Failed to close epoll file descriptor\n");
			return 1;
		}
#endif
	}

	// with input
	else
	{
	}

	exit(EXIT_SUCCESS);
}