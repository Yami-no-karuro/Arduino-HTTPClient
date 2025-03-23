#include "WiFiS3.h"
#include "./secrets.h"

#define SERVER_CONNECTION_RETRY_COUNT 5
#define SERVER_CONNECTION_RETRY_DELAY 5000
#define SERVER_RESPONSE_TIMEOUT 5000
#define WIFI_CONNECTION_RETRY_DELAY 5000

WiFiClient client;

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
 * Executed a GET HTTP request to server. 
 * 
 * @param server - The server IP address or domain name.
 * @param path - The request path.
 */
void client_get(char *server, char *path)
{
  if (!client.connected())
    return;

  char request_line[256];
  sprintf(request_line, "GET %s HTTP/1.1", path);
  client.println(request_line);

  char host_line[256];
  sprintf(host_line, "Host: %s", server);
  client.println(host_line);

  client.println("User-Agent: Arduino UNO R4 Wifi");
  client.println("Connection: close");
  client.println();
}

/**
 * Reads the response and prints it to the serial monitor.
 */
void client_read_response()
{
  unsigned long start_time = millis();
  while (millis() - start_time < SERVER_RESPONSE_TIMEOUT) {
    if (client.available()) {
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
      }

      return;
    }
  }

  Serial.println("Warning: Server response timeout");
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
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  char path[] = "/";
  client_get(server, path);

  client_read_response();
  client_disconnect();
  while (true);
}
