#include <string.h>

#include "WiFiS3.h"
#include "./secrets.h"

#define WIFI_CONNECTION_RETRY_DELAY 5000

#define SERVER_CONNECTION_RETRY_COUNT 5
#define SERVER_CONNECTION_RETRY_DELAY 5000
#define SERVER_RESPONSE_TIMEOUT 5000
#define SERVER_RESPONSE_BUFFER_SIZE 1024

WiFiClient client;

typedef struct {
    char *key;
    char *value;
} PostParam;

/**
 * Connects to the provided IP address (or domain name) and port. 
 * 
 * @param server - The server IP address or domain name.
 * @param port - The server port.
 * @return - "true" on success, "false" on failure.
 */
bool client_connect(char *server, int port)
{
  for (int i = 0; i < SERVER_CONNECTION_RETRY_COUNT; i++) {
    if (client.connect(server, port))
      return true;

    delay(SERVER_CONNECTION_RETRY_DELAY);
  }

  return false;
}

/**
 * Disconnects from the server.
 * 
 * @param flush - Whether to flush the unread client data.
 */
void client_disconnect(bool flush = true)
{
  if (flush)
    client.flush();

  client.stop();
}

/**
 * Executes a GET HTTP request.
 * 
 * @param server - The server IP address or domain name.
 * @param path - The request path.
 */
void client_get(char *server, char *path)
{
  if (!client.connected())
    return;

  char request_line[256];
  snprintf(request_line, sizeof(request_line), "GET %s HTTP/1.1", path);
  client.println(request_line);

  char host_line[256];
  snprintf(host_line, sizeof(host_line), "Host: %s", server);
  client.println(host_line);

  client.println("Connection: keep-alive");
  client.println("User-Agent: Arduino UNO R4 Wifi");
  client.println();
}

/**
 * Executes a POST HTTP request. 
 * 
 * @param server - The server IP address or domain name.
 * @param path - The request path.
 * @param params - The POST parameters in "x-www-form-urlencoded".
 * @param num_params - The number of POST parameters.
 */
void client_post(char *server, char *path, PostParam *params, size_t num_params)
{
  if (!client.connected())
    return;

  char request_line[256];
  snprintf(request_line, sizeof(request_line), "POST %s HTTP/1.1", path);
  client.println(request_line);

  char host_line[256];
  snprintf(host_line, sizeof(host_line), "Host: %s", server);
  client.println(host_line);

  char body[1024] = "";
  for (size_t i = 0; i < num_params; i++) {
    char param[256];
    snprintf(param, sizeof(param), "%s=%s", params[i].key, params[i].value);
    strncat(body, param, sizeof(body));

    if (i < (num_params - 1))
      strncat(body, "&", sizeof(body));
  }

  client.println("Connection: keep-alive");
  client.println("User-Agent: Arduino UNO R4 Wifi");
  client.println("Content-Type: application/x-www-form-urlencoded");

  char length_line[256];
  snprintf(length_line, sizeof(length_line), "Content-Length: %ld\n\n", strlen(body));
  client.print(length_line);

  client.println(body);
  client.println();
}

/**
 * Reads the response into a buffer.
 * 
 * @param buffer - The buffer where the response will be stored.
 * @param buffer_size - The maximum size of the buffer.
 * @return - True if a response was received, False if timed out.
 */
bool client_read_response(char *buffer, size_t buffer_size)
{
  size_t index = 0;
  unsigned long start_time = millis();

  while (millis() - start_time < SERVER_RESPONSE_TIMEOUT) {
    while (client.available()) {
      if (index < buffer_size - 1) {
        buffer[index++] = client.read();
      } else {
        buffer[index] = '\0';
        Serial.println("Warning: Response buffer overflow");
        return false;
      }
    }

    if (index > 0) {
      buffer[index] = '\0';
      return true;
    }
  }

  Serial.println("Warning: Server response timeout");
  return false;
}

void execute_post_request()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  PostParam params[] = {
    {"foo", "bar"},
    {"fizz", "buzz"}
  };

  char path[] = "/";
  size_t num_params = sizeof(params) / sizeof(params[0]);
  client_post(server, path, params, num_params);

  char response[SERVER_RESPONSE_BUFFER_SIZE];
  if (client_read_response(response, SERVER_RESPONSE_BUFFER_SIZE))
    Serial.println(response);

  client_disconnect();
}

void execute_get_request()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  char path[] = "/";
  client_get(server, path);

  char response[SERVER_RESPONSE_BUFFER_SIZE];
  if (client_read_response(response, SERVER_RESPONSE_BUFFER_SIZE))
    Serial.println(response);

  client_disconnect();
}

/**
 * ====
 * Setup
 * ====
 */
void setup() 
{
  Serial.begin(9600);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Error: Unable to communicate with the \"Wifi\" module");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    Serial.println("Warning: Firmware update available");

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Info: Trying to connect to SSID ");
    Serial.println(NET_SSID);

    status = WiFi.begin(NET_SSID, NET_PASS);
    delay(WIFI_CONNECTION_RETRY_DELAY);
  }

  Serial.print("Info: Connected to SSID ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Info: Local IP Address ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Info: Signal Strength (RSSI) ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

/**
 * ====
 * Loop 
 * ====
 */
void loop() 
{
  execute_get_request();
  execute_post_request();

  while (true);
}
