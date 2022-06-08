



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <thread>
#include <string>

// import Delta;
// import Async;


using std::thread, std::cout, std::endl;

// #define E

// static thread_local auto tt = std::string {};

int main(int, char **)
{
	auto new_data = [] (auto sockfd) noexcept -> auto {
		auto txt = std::string {};
		txt.resize (1021);
		// cout << txt.capacity () << endl;
		auto nbytes = read (sockfd, txt.data(), txt.capacity ());
		txt [nbytes] = '\0';
		// cout << txt << endl;
		return txt;
		// auto nBytes = read (sockfd, )
	};

	auto handle_data = [] (auto sockfd, std::string&& txt) noexcept -> auto {

		auto i = txt.find (' ');

		if (i == std::string::npos) {
			cout << "error" << endl;
			return;
		}

		auto method = std::string {txt.begin(), txt.begin() + i};

		cout << std::string {txt.begin() + i + 1, txt.end()} << endl;
		
		decltype (i) j;

		if (j = std::string {txt.begin() + i + 1, txt.end()}.find(' '); j == std::string::npos) {
			cout << "error" << endl;
			return;
		}

		j += i + 1;

		auto path = std::string {txt.begin() + i + 1, txt.begin() + j};

		i = j;

		if (j = std::string {txt.begin() + i + 1, txt.end()}.find ('\r'); j == std::string::npos){
			cout << "error" << endl;
			return;
		}

		j += i + 1;

		auto version = std::string {txt.begin() + i + 1, txt.begin() + j};

		
		cout << "method:" << method << endl << "path:" << path << endl << "version:" << version << endl;;
	};


    auto as_server = [port = "80"] (auto newData, auto handleData)
    {
        int sockfd;
        int epollfd;

        addrinfo *servinfo{nullptr};
        auto hints = addrinfo{};

        memset(&hints, 0, sizeof hints);

        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if (auto r = getaddrinfo(NULL, port, &hints, &servinfo); r != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
            exit(-1);
        }

        auto *i = servinfo;

        // loop through all the results and connect to the first we can
        for (; i != NULL; i = i->ai_next)
        {
            if ((sockfd = socket(i->ai_family, i->ai_socktype,
                                 i->ai_protocol)) == -1)
            {
                perror("socket");
                continue;
            }

            if (int yes = 1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            {
                perror("setsockopt");
                continue;
            }

            // fcntl (_sockfd, F_SETFL, O_NONBLOCK | FASYNC);

            if (bind(sockfd, i->ai_addr, i->ai_addrlen) == -1)
            {
                close(sockfd);
                perror("server: bind");
                continue;
            }

            break;
        }

        freeaddrinfo(servinfo);

        if (i == NULL)
        {
            printf ("client failed to connect\n");
            exit(1);
        }

        if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
        {
            perror("fcntl");
            exit(1);
        }

        if (listen(sockfd, 20) == -1)
        {
            perror("listen");
            exit(1);
        }

        if ((epollfd = epoll_create1(0)) == -1)
        {
            perror("epoll_create1");
            exit(1);
        }

        auto event = epoll_event {};
        event.events = EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.fd = sockfd;

        if (epoll_ctl (epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1) 
        {
            perror ("epoll_ctl");
            exit (1);
        }

        epoll_event events [32];
        
        auto keep_running = true;

		char buf [1024];

        while (keep_running)
        {
			cout << "waiting..." << endl;
            auto nfds = epoll_wait (epollfd, events, 32, -1);
			// cout << "yay" << endl;
            for (auto i = 0; i < nfds; ++i)
            {
                if (events[i].data.fd == sockfd) 
                {
					// async::out << "new connection\n";
					cout << "new connection" << endl;
                    /* handle new connection */
                    struct sockaddr_in cli_addr;
                    socklen_t socklen = sizeof (cli_addr);
                    auto client_sockfd = accept (sockfd, (struct sockaddr *)&cli_addr, &socklen);

                    if (fcntl (client_sockfd, F_SETFD, fcntl(client_sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
                    {
                        perror("fcntl");
                        exit(1);
                    }

                    // inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr),
                    //     buf, sizeof(cli_addr));
                    // printf("[+] connected with %s:%d\n", buf,
                    //     ntohs(cli_addr.sin_port));

                    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
                    event.data.fd = client_sockfd;

                    if (epoll_ctl (epollfd, EPOLL_CTL_ADD, client_sockfd, &event) == -1) 
                    {
                        perror ("epoll_ctl");
                        exit (1);
                    }
                    
                } else if (events[i].events & EPOLLIN) // we have incoming message from a socket
                {
					auto sock = events[i].data.fd;
					handleData (sock, newData (sock));

                } else if (events[i].events & (EPOLLRDHUP | EPOLLHUP))
                {
                    cout << "hehe" << endl;
                }
            }
        }
    };

	as_server (new_data, handle_data);

	// auto test_range = [] (Range auto const&)
	// {

	// };

	
	// async::out << "hej\n";
	// cout << "hej\n";
	
	
	
	// auto t2 = (long) t1;
	// auto t3 = t2 + sizeof (int);
	// auto* t4 = (int*) t3; 
	// cout << t1+1 << endl << t4 << endl;
	

	// auto t4 = (int*) ((long) t1 + sizeof (int))
	



    return 0;
}
