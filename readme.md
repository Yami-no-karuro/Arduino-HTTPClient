# Arduino - HTTPClient

## Simple WiFiS3 based HTTP client

### Introduction

This project aims to be a "template" for applications that require specific interaction with [HTTP](https://en.wikipedia.org/wiki/HTTP) web services.  
The goal is to create a simple way to execute [HTTP](https://en.wikipedia.org/wiki/HTTP) requests through some high-level functions built on top of the [WiFiS3](https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/WiFiS3) library.  
In the following examples, for demonstration purposes, [requestcatcher.com](https://requestcatcher.com/) services were used.

### Credentials

The `HTTP-Client.ino` sketch includes `secrets.h` and makes use of the `NET_SSID` and `NET_PASS` constants.  
To make everything work properly create your own `secrets.h` like illustrated below.

```c
#define NET_SSID "<ssid>"
#define NET_PASS "<password>"
```

### Globals & Fine-Tuning

The program make use of some specific constants such as `SM_BUFFER` stored in the `src/globals.h` file.  
The default values have been configured to be as generic as possible, before using the wrapper in a real case it is a good idea to fine tune those values to best fit the use case.

### Basics

Every [HTTP](https://en.wikipedia.org/wiki/HTTP) interaction always starts with `client.httpConnect()` and must always end with `client.httpDisconnect()`.  
The `client.httpConnect()` function wraps the standard `client.connect()` call to provide **retry-on-failure** functionalities.  
By default the program awaits `SERVER_CONNECTION_RETRY_DELAY` (5000) for `SERVER_CONNECTION_RETRY_COUNT` (5) times.  
Let's have a look at a simple connection configuration.

```c
  int port = 80;                                       // The server port, usually 80 (TCP/HTTP) or 443 (TCP/HTTPS).
  char server[] = "arduino-demo.requestcatcher.com";   // The server domain or IP address.
  if (!client.httpConnect(server, port))
    return;

  // Request - Response interactions.
  // ... 

  client.httpDisconnect();
```

### Executing HTTP Requests via wrapper API

The `client.cpp` wrapper supports basic **GET** and **POST** requests via `client.httpGet()` and `client.httpPost()`.  
Let's look at both cases in detail.

**GET** request.  
The `client.httpGet()` function accepts 2 parameters:

- `server`, the remote IP address or domain name.
- `port`, the remote port.

```c
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

    client.httpDisconnect();
  }
```

**POST** request.
The `client.httpPost()` function accepts 4 parameters:

- `server`, the remote IP address or domain name.
- `port`, the remote port.
- `payload`, the POST request payload.
- `content_type`, the payload content-type.

```c
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

    client.httpDisconnect();
  }
```

### Reading the HTTP Response

The `client.cpp` wrapper supports a basic way to read the server response to a buffer.  
Let's extend the previous **GET** request example to include response reading.

```c
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

    // Reads the response and prints it on the serial.
    char response[LG_BUFFER];
    if (client.httpReadResponse(response, LG_BUFFER))
      Serial.println(response);

    client.httpDisconnect();
  }
```

### Compile & Upload

Pre-made `sh` commands are available for compiling (`compile.sh`) and uploading (`upload.sh`) the code.  
Before using the commands make sure the configurations are correct.

```bash
#!/bin/bash

# ====
# Global variables
# ====

FQBN=arduino:renesas_uno:unor4wifi  # <-- Enter the desired FQBN, should be something like "arduino:renesas_uno:unor4wifi"
BUILD_PATH=build                    # <-- Enter the desired build directory path, should be somethind like "build"
```
