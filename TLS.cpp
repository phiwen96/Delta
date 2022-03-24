module;
#include <time.h>
// #include <cstring>
#include <cstdlib>
#include <cassert>
// #include <string.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <string.h>
#include <sys/socket.h>
#include <cstring>
#include <stdio.h>

export module Delta.Net.TLS;

import Delta.CopyPaste;

#if BYTE_ORDER == BIG_ENDIAN

#define HTONS(n) (n)
#define NTOHS(n) (n)
#define HTONL(n) (n)
#define NTOHL(n) (n)

#else

#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define NTOHS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define NTOHL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))
#endif

#define htons(n) HTONS(n)
#define ntohs(n) NTOHS(n)

#define htonl(n) HTONL(n)
#define ntohl(n) NTOHL(n)


#define TLS_VERSION_MAJOR 3
#define TLS_VERSION_MINOR 1



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

enum class cipher_suites_identifier : int
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

auto append_buffer( auto *dest, auto *src, size_t n ) -> char*
{
    // memcpy( (void*) dest, (void * const) src, n );
    copy_paste (src, dest, n);
    return dest + n;
}

/**
* Handshake message types (section 7.4)
*/
enum class handshake_type : int
{
    hello_request = 0,
    client_hello = 1,
    server_hello = 2,
    certificate = 11,
    server_key_exchange = 12,
    certificate_request = 13,
    server_hello_done = 14,
    certificate_verify = 15,
    client_key_exchange = 16,
    finished = 20
};

/**
* Handshake record definition (section 7.4)
*/
struct handshake
{
    unsigned char msg_type;
    unsigned int length;
// 24 bits(!)
};

/** This lists the type of higher-level TLS protocols that are defined */
enum class content_type : int
{
    content_change_cipher_spec = 20,
    content_alert = 21,
    content_handshake = 22,
    content_application_data = 23
};
enum class alert_level : int { warning = 1, fatal = 2 };
/**
* Enumerate all of the error conditions specified by TLS.
*/
enum class alert_description : int
{
    close_notify = 0,
    unexpected_message = 10,
    bad_record_mac = 20,
    decryption_failed = 21,
    record_overflow = 22,
    decompression_failure = 30,
    handshake_failure = 40,
    bad_certificate = 42,
    unsupported_certificate = 43,
    certificate_revoked = 44,
    certificate_expired = 45,
    certificate_unknown = 46,
    illegal_parameter = 47,
    unknown_ca = 48,
    access_denied = 49,
    decode_error = 50,
    decrypt_error = 51,
    export_restriction = 60,
    protocol_version = 70,
    insufficient_security = 71,
    internal_error = 80,
    user_canceled = 90,
    no_renegotiation = 100
};
struct alert
{
    unsigned char level;
    unsigned char description;
};
/**
* Each packet to
*/
struct tls_plaintext
{
    protocol_version version;
    unsigned char type;
    unsigned short length;
};

export struct tls 
{  
    tls (int sockfd)
    {
        memset (master_secret, '\0', master_secret_length);
        memset (client_random, '\0', random_length);
        memset (server_random, '\0', random_length);

        send_client_hello (sockfd);

        receive_tls_message (sockfd);
        
    }
    
private:
    auto send_message (int connection, content_type conttype, const unsigned char *content, short content_len) -> void
    {
        
        tls_plaintext header;
        unsigned char *send_buffer;
        int send_buffer_size;
        send_buffer_size = content_len;
        send_buffer_size +=5;
        send_buffer = ( unsigned char * ) malloc( send_buffer_size );
        
        header.type = (int) conttype;
        header.version.major = TLS_VERSION_MAJOR;
        header.version.minor = TLS_VERSION_MINOR;
        header.length = htons( content_len );
        send_buffer[ 0 ] = header.type;
        send_buffer[ 1 ] = header.version.major;
        send_buffer[ 2 ] = header.version.minor;
        
        copy_paste (&header.length, send_buffer + 3, sizeof (short));
        copy_paste (content, send_buffer + 5, content_len);
        // memcpy( send_buffer + 3, &header.length, sizeof( short ) );
        // memcpy( send_buffer + 5, content, content_len );
        if ( send (connection, (void *) send_buffer, send_buffer_size, 0) < send_buffer_size)
        {
            perror ("send");
        }

        free (send_buffer);
        
    }
    auto send_handshake_message (int sockfd, handshake_type msg_type, /*const unsigned char **/ auto* message, int message_len) -> void
    {
        handshake record;
        short send_buffer_size;
        unsigned char *send_buffer;

        record.msg_type = (int) msg_type;
        record.length = htons (message_len) << 8; // To deal with 24-bits...
        send_buffer_size = message_len + 4; // space for the handshake header
        send_buffer = (unsigned char *) malloc (send_buffer_size);
        send_buffer[ 0 ] = record.msg_type;
        copy_paste (&record.length, send_buffer + 1, 3);
        copy_paste (message, send_buffer + 4, message_len);
        // memcpy (send_buffer + 1, &record.length, 3);
        // memcpy (send_buffer + 4, message, message_len);
        send_message (sockfd, content_type::content_handshake, send_buffer, send_buffer_size);
        free (send_buffer);
    }
    auto send_client_hello (int sockfd) -> void
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

        assert ((( char * ) write_buffer - send_buffer ) == send_buffer_size);
        send_handshake_message (sockfd, handshake_type::client_hello, send_buffer, send_buffer_size);
        free (send_buffer);
        
    }
    
    auto receive_tls_message (int sockfd) -> void
    {
        tls_plaintext message;
        unsigned char *read_pos, *msg_buf;
        unsigned char header[ 5 ]; // size of TLSPlaintext
        int bytes_read, accum_bytes;
        // STEP 1 - read off the TLS Record layer
        if ( recv( sockfd, header, 5, 0 ) <= 0 )
        {
            // No data available; it’s up to the caller whether this is an error or not.
            perror ("recv");
            exit (-1);
        }
        message.type = header[ 0 ];
        message.version.major = header[ 1 ];
        message.version.minor = header[ 2 ];
        copy_paste (header + 3, &message.length, 2);
        message.length = htons( message.length );
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


