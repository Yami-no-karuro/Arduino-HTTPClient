#ifndef CLIENT_H
#define CLIENT_H

#include "WiFiS3.h"

#define SERVER_CONNECTION_RETRY_COUNT 5
#define SERVER_CONNECTION_RETRY_DELAY 5000
#define SERVER_RESPONSE_TIMEOUT 5000

typedef struct {
    char *key;
    char *value;
} PostParam;

class HTTPClient: public WiFiClient 
{
    public:
        bool httpConnect(const char *server, int port);
        void httpDisconnect();
        void httpGet(const char *server, const char *path);
        void httpPost(const char *server, const char *path, PostParam *params, size_t num_params);
        bool httpReadResponse(char *buffer, size_t buffer_size);
};

#endif
