#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_CONNECTION_RETRY_COUNT 5
#define SERVER_CONNECTION_RETRY_DELAY 5000
#define SERVER_RESPONSE_TIMEOUT 5000

typedef struct {
    char *key;
    char *value;
} PostParam;

bool client_connect(char *server, int port);
void client_disconnect();
void client_get(char *server, char *path);
void client_post(char *server, char *path, PostParam *params, size_t num_params);
bool client_read_response(char *buffer, size_t buffer_size);

#endif
