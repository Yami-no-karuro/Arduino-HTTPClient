#include <string.h>
#include "WiFiS3.h"
#include "client.h"
#include "../globals.h"

extern WiFiClient client;

/**
 * Connects to the provided IP Address (or domain name via DNS lookup) and port.
 * 
 * @param server - The server IP address or domain name.
 * @param port - The server port.
 * @return - true on success, false on failure.
 */
bool HTTPClient::httpConnect(const char *server, int port)
{
    for (size_t i = 0; i < SERVER_CONNECTION_RETRY_COUNT; i++) {
        if (this->connect(server, port))
            return true;

        delay(SERVER_CONNECTION_RETRY_DELAY);
    }

    Serial.println("Error: Server connection timeout.");
    return false;
}

/**
 * Closes the connection (client content is flushed).
 */
void HTTPClient::httpDisconnect()
{
    this->flush();
    this->stop();
}

/**
 * Executes a GET HTTP request.
 * 
 * @param server - The server IP address or domain name.
 * @param path - The request path.
 */
void HTTPClient::httpGet(const char *server, const char *path)
{
    if (!this->connected())
        return;

    char request_line[SM_BUFFER];
    snprintf(request_line, sizeof(request_line), "GET %s HTTP/1.1", path);
    this->println(request_line);

    char host_line[SM_BUFFER];
    snprintf(host_line, sizeof(host_line), "Host: %s", server);
    this->println(host_line);

    this->println("User-Agent: Arduino UNO R4 WiFi");
    this->println();
}

/**
 * Executes a POST HTTP request. 
 * 
 * @param server - The server IP address or domain name.
 * @param path - The request path.
 * @param params - The POST parameters in "x-www-form-urlencoded" format.
 * @param num_params - The number of POST parameters.
 */
void HTTPClient::httpPost(const char *server, const char *path, PostParam *params, size_t num_params)
{
    if (!this->connected())
        return;

    char request_line[SM_BUFFER];
    snprintf(request_line, sizeof(request_line), "POST %s HTTP/1.1", path);
    this->println(request_line);

    char host_line[SM_BUFFER];
    snprintf(host_line, sizeof(host_line), "Host: %s", server);
    this->println(host_line);

    char body[MD_BUFFER] = "";
    for (size_t i = 0; i < num_params; i++) {
        char param[SM_BUFFER];
        snprintf(param, sizeof(param), "%s=%s", params[i].key, params[i].value);
        strncat(body, param, sizeof(body));

        if (i < (num_params - 1))
            strncat(body, "&", sizeof(body));
    }

    this->println("User-Agent: Arduino UNO R4 WiFi");
    this->println("Content-Type: application/x-www-form-urlencoded");

    char length_line[SM_BUFFER];
    snprintf(length_line, sizeof(length_line), "Content-Length: %ld\n\n", strlen(body));
    this->print(length_line);

    this->println(body);
    this->println();
}

/**
 * Reads the HTTP Response into a buffer.
 * 
 * @param buffer - The buffer where the response will be stored.
 * @param buffer_size - The maximum size of the buffer.
 * @return - true if a response was received, false if timed out.
 */
bool HTTPClient::httpReadResponse(char *buffer, size_t buffer_size)
{
    size_t index = 0;
    unsigned long start_time = millis();

    while (millis() - start_time < SERVER_RESPONSE_TIMEOUT) {
        while (this->available()) {
            if (index < buffer_size - 1) {
                buffer[index++] = this->read();
                continue;
            }

            buffer[index] = '\0';
            Serial.println("Error: Response buffer overflow.");
            return false;
        }

        if (index > 0) {
            buffer[index] = '\0';
            return true;
        }
    }

    Serial.println("Error: Server response timeout.");
    return false;
}
