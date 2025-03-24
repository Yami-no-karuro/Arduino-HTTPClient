#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
    char *key;
    char *value;
} PostParam;

bool client_connect(char *server, int port);
void client_disconnect(bool flush = true);
void client_get(char *server, char *path);
void client_post(char *server, char *path, PostParam *params, size_t num_params);
bool client_read_response(char *buffer, size_t buffer_size);

#endif
