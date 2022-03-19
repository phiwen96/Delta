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
    constexpr auto port = "443";
    constexpr auto ip = "google.com";

    struct addrinfo hints, *servinfo, *p;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (auto i = getaddrinfo (ip, port, &hints, &servinfo); i != 0)
    {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (i));
    }

    


    Char auto c = 'g';
    String auto str = "hej"; 

    async_write (1, str, 4);
    // while(true){} 
    return 0;
}

