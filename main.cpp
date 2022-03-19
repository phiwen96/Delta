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


int main (int, char**) 
{
    return 0;
    constexpr auto port = "443";
    constexpr auto ip = "google.com";

    struct addrinfo hints, *servinfo, *p;
    int sockfd;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (auto i = getaddrinfo (ip, port, &hints, &servinfo); i != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (i));
    }

    for (p = servinfo; p != NULL; p = p -> ai_next)
    {
        if ((sockfd = socket (p -> ai_family, p -> ai_socktype, p -> ai_protocol)) == -1)
        {
            perror ("socket");
            continue;
        }

        if (connect (sockfd, p -> ai_addr, p -> ai_addrlen) == -1)
        {
            close (sockfd);
            perror ("connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf (stderr, "failed to connect");
        exit (1);
    }

    freeaddrinfo (servinfo);

    int numbytes;

    char buf [4096];

    if ((numbytes = recv (sockfd, buf, 4095, 0)) == -1)
    {
        perror ("recv");
        exit (1);
    }

    buf [numbytes] = '\0';

    printf (buf);

    close (sockfd);

    Char auto c = 'g';
    String auto str = "hej"; 

    async_write (1, str, 4);
    // while(true){} 
    return 0;
}

