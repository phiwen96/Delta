// Transport Layer Security

export module Delta.Net.TLS;


// export auto tls (auto sockid) noexcept -> bool 
// {

// }




struct protocol_version
{
    unsigned char major, minor;
};

//  helps to guard against replay arracks
struct random 
{
    unsigned int gmt_unix_time;
    unsigned char random_bytes [28];
};

struct protected_params 
{
    unsigned char* MAC_secret {nullptr};
    unsigned char* key {nullptr}; // symmetric encryption key
    unsigned char* IV {nullptr}; // initialization vector
};


struct tls 
{  

private:
    protected_params pending_send_params;
    protected_params pending_recv_params;
    protected_params active_send_params;
    protected_params active_recv_params;

    
};


