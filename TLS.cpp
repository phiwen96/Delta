// Transport Layer Security

module;
#include "time.h"
export module Delta.Net.TLS;

struct protocol_version
{
    unsigned char major, minor;
};

//  helps to guard against replay arracks
struct random 
{
    unsigned int gmt_unix_time;
    unsigned char random_bytes [28];
    
    random ()
    {
        auto local_time = time_t {};
        time (&local_time);
        gmt_unix_time = htonl (local_time);
    }
};

struct client_hello
{
    protocol_version client_version;
    random randoom;
    
};

constexpr auto master_secret_length = 48;
using master_secret_type = unsigned char [master_secret_length];

constexpr auto random_length = 48;
using random_type = unsigned char [random_length];

struct protected_params 
{
    unsigned char* MAC_secret {nullptr};
    unsigned char* key {nullptr}; // symmetric encryption key
    unsigned char* IV {nullptr}; // initialization vector
};

struct tls 
{  
    tls (int sockid)
    {
        
    }
private:
    protected_params pending_send_params;
    protected_params pending_recv_params;
    protected_params active_send_params;
    protected_params active_recv_params;

    
};


export auto send (int sockfd, tls& params, String auto const& msg) -> void
{
    
}


