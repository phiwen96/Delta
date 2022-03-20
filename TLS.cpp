// Transport Layer Security

module;
#include <time.h>
#include <stdio.h>
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
    client_hello (protocol_version const& client)
    {
        
    }
    protocol_version client_version;
    random randoom;
    unsigned char session_id_length;
    unsigned char* session_id;
    unsigned short cipher_suites_length;
    unsigned short* cipher_suites;
    unsigned char compression_methods_length;
    unsigned char* compression_methods;
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
        memset (master_secret, '\0', master_secret_length);
        memset (client_random, '\0', random_length);
        memset (server_random, '\0', random_length);
    }
    
private:
    auto send_client_hello ()
    {
        auto package = client_hello {};
        
    }
    
    master_secret_type master_secret;
    random_type client_random;
    random_type server_random;
    
    protected_params pending_send_params;
    protected_params pending_recv_params;
    protected_params active_send_params;
    protected_params active_recv_params;
    
  // RSA public key, if supplied
  // public_key_info       server_public_key;
  // DH public key, if supplied (either in a certificate or ephemerally)
  // Note that a server can legitimately have an RSA key for signing and
  // a DH key for key exchange (e.g. DHE_RSA)
  // dh_key                server_dh_key
};


export auto send (int sockfd, tls& params, String auto const& msg) -> void
{
    
}


