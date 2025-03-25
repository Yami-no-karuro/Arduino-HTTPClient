#include "WiFiS3.h"
#include "secrets.h"
#include "src/globals.h"
#include "src/lib/client.h"

float rand_float(float min, float max)
{
    return min + ((float) rand() / (float) RAND_MAX) * (max - min);
}

void post_request_demo()
{
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  if (!client_connect(server, port))
    return;

  // Example...
  // Sending coordinates via latitude (lat) and longitude (log) parameters to the server.
  // Using 4 decimal to simulate ~10m GPS tolerance.

  srand(millis());

  float lat = rand_float(-90.0f, 90.0f);
  float lon = rand_float(-180.0f, 180.0f);
  unsigned long timestamp = millis();

  char lat_str[XXS_BUFFER];
  snprintf(lat_str, sizeof(lat_str), "%.4f", lat);

  char lon_str[XXS_BUFFER];
  snprintf(lon_str, sizeof(lon_str), "%.4f", lon);

  char timestamp_str[XXS_BUFFER];
  snprintf(timestamp_str, sizeof(timestamp_str), "%ld", timestamp);

  PostParam params[] = {
    {"lat", lat_str},
    {"lon", lon_str},
    {"time", timestamp_str}
  };

  char path[] = "/api/v1/coordinates";
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
  if (!client_connect(server, port))
    return;

  // Example...
  // Requesting some sort of system logs from the server.

  char path[] = "/api/v1/logs";
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
  Serial.println(" dBm.");
}

void loop() 
{
  get_request_demo();
  post_request_demo();

  while (true);
}
