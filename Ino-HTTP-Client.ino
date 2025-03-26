#include "WiFiS3.h"
#include "secrets.h"
#include "src/globals.h"
#include "src/lib/client.h"
#include "src/lib/utils.h"

HTTPClient client;

void post_request_demo()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  if (!client.httpConnect(server, port))
    return;

  // Example...
  // Sending some sort of coordinates via latitude (lat) and longitude (log) to the server.
  // Using 4 decimal to simulate ~10m GPS tolerance.

  srand(millis());
  float lat = rand_float(-90.0f, 90.0f);
  float lon = rand_float(-180.0f, 180.0f);

  char payload[MD_BUFFER];
  snprintf(payload, sizeof(payload), "lat=%.4f&lon=%.4f", lat, lon);

  char path[] = "/api/v1/coordinates";
  char content_type[] = "application/x-www-form-urlencoded";
  client.httpPost(server, path, payload, content_type);

  char response[LG_BUFFER];
  if (client.httpReadResponse(response, LG_BUFFER))
    Serial.println(response);

  client.httpDisconnect();
}

void get_request_demo()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  if (!client.httpConnect(server, port))
    return;

  // Example...
  // Requesting some sort of system logs from the server.

  char path[] = "/api/v1/logs";
  client.httpGet(server, path);

  char response[LG_BUFFER];
  if (client.httpReadResponse(response, LG_BUFFER))
    Serial.println(response);

  client.httpDisconnect();
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
  Serial.println(" dBm.");
}

void loop() 
{
  get_request_demo();
  post_request_demo();

  while (true);
}
