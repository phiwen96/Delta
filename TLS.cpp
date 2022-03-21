module;
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
export module Delta.Net.TLS;

// Transport Layer Security

struct protocol_version
{
    unsigned char major, minor;
};

//  helps to guard against replay arracks
struct random_t
{
    unsigned int gmt_unix_time;
    unsigned char random_bytes [28];
    
    random_t ()
    {
        auto local_time = time_t {};
        time (&local_time);
        gmt_unix_time = htonl (local_time);
    }
};

struct client_hello
{
    protocol_version client_version;
    random_t randoom;
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

enum cipher_suites_identifier
{
    TLS_NULL_WITH_NULL_NULL = 0x0000,
    TLS_RSA_WITH_NULL_MD5 = 0x0001,
    TLS_RSA_WITH_NULL_SHA = 0x0002,
    TLS_RSA_EXPORT_WITH_RC4_40_MD5 = 0x0003,
    TLS_RSA_WITH_RC4_128_MD5 = 0x0004,
    TLS_RSA_WITH_RC4_128_SHA = 0x0005,
    TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5 = 0x0006,
    TLS_RSA_WITH_IDEA_CBC_SHA = 0x0007,
    TLS_RSA_EXPORT_WITH_DES40_CBC_SHA = 0x0008,
    TLS_RSA_WITH_DES_CBC_SHA = 0x0009,
    TLS_RSA_WITH_3DES_EDE_CBC_SHA = 0x0010
};

auto append_buffer( char *dest, char *src, size_t n ) -> char*
{
    memcpy( dest, src, n );
    return dest + n;
}

export struct tls 
{  
    tls (int sockid)
    {
        memset (master_secret, '\0', master_secret_length);
        memset (client_random, '\0', random_length);
        memset (server_random, '\0', random_length);

        send_client_hello (sockid);
    }
    
private:
    auto send_client_hello (int sockid) -> void
    { 
        unsigned short supported_suites [1] = {htons (cipher_suites_identifier::TLS_RSA_WITH_3DES_EDE_CBC_SHA)};
        unsigned char supported_compression_methods [1] = {0};
        

        auto package = client_hello 
        {
            .session_id_length = 0,
            .session_id = nullptr,
            .cipher_suites_length = htons (2),
            .cipher_suites = supported_suites,
            .compression_methods_length = 1,
            .compression_methods = supported_compression_methods
        };
        
        // Compute the size of the ClientHello message after flattening.
        int send_buffer_size = sizeof( protocol_version ) + sizeof( random_t ) + sizeof( unsigned char ) + ( sizeof( unsigned char ) * package.session_id_length ) + sizeof( unsigned short ) + ( sizeof( unsigned short ) * 1 ) + sizeof( unsigned char ) + sizeof( unsigned char );
        char* send_buffer = (char*) malloc (send_buffer_size);
        void* write_buffer = send_buffer;

        write_buffer = append_buffer( (char*) write_buffer, ( char * ) &package.client_version.major, 1 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * )&package.client_version.minor, 1 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * )&package.randoom.gmt_unix_time, 4 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * )&package.randoom.random_bytes, 28 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * )&package.session_id_length, 1 );

        if ( package.session_id_length > 0 )
        {
            write_buffer = append_buffer( (char*) write_buffer, ( char * )package.session_id, package.session_id_length );
        }

        write_buffer = append_buffer( (char*) write_buffer, ( char * ) &package.cipher_suites_length, 2 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * ) package.cipher_suites, 2 );
        write_buffer = append_buffer( (char*) write_buffer, ( char * ) &package.compression_methods_length, 1 );

        if ( package.compression_methods_length > 0 )
        {
            write_buffer = append_buffer( (char*) write_buffer, ( char * ) package.compression_methods, 1 );
        }

        
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


export auto send (int sockfd, tls& params, char const* msg) -> void
{
    
}


