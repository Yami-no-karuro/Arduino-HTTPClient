#include "WiFiS3.h"

#include "./secrets.h"
#include "./globals.h"
#include "./client.h"

void post_request_demo()
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

  char response[LG_BUFFER];
  if (client_read_response(response, LG_BUFFER))
    Serial.println(response);

  client_disconnect();
}

void get_request_demo()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  char path[] = "/";
  client_get(server, path);

  char response[LG_BUFFER];
  if (client_read_response(response, LG_BUFFER))
    Serial.println(response);

  client_disconnect();
}

void setup() 
{
  Serial.begin(9600);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Error: Unable to communicate with the \"Wifi\" module.");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    Serial.println("Warning: Firmware update available.");

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Info: Trying to connect to SSID \"");
    Serial.print(NET_SSID);
    Serial.println("\".");

    status = WiFi.begin(NET_SSID, NET_PASS);
    delay(5000);
  }

  Serial.print("Info: Connected to SSID \"");
  Serial.print(WiFi.SSID());
  Serial.println("\".");

  IPAddress ip = WiFi.localIP();
  Serial.print("Info: Local IP Address \"");
  Serial.print(ip);
  Serial.println("\".");

  long rssi = WiFi.RSSI();
  Serial.print("Info: Signal Strength (RSSI) ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void loop() 
{
  get_request_demo();
  post_request_demo();

  // ...
  while (true);
}
