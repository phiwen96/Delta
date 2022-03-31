import Delta;
// import Async;
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

using std::thread, std::cout, std::endl;

int main(int, char **)
{
	auto as_client = []
    {
        constexpr auto port = "443";
        constexpr auto ip = "127.0.0.1";

        struct addrinfo hints, *servinfo, *p;
        int sockfd;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if (auto i = getaddrinfo(ip, port, &hints, &servinfo); i != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(i));
        }

        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                perror("socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
            {
                close(sockfd);
                perror("connect");
                continue;
            }

            break;
        }

        if (p == NULL)
        {
            fprintf(stderr, "failed to connect");
            exit(1);
        }

        freeaddrinfo(servinfo);

        auto secured = tls{sockfd};

        int numbytes;

        char buf[4096];

		// async::out << "receiving\n";

        if ((numbytes = recv(sockfd, buf, 4095, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        close(sockfd);
    };

    



    

    

    return 0;
}
