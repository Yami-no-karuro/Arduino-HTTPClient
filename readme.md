# Arduino - HTTP-Client 

## Simple HTTP client functionalities

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

### Credentials

The **Ino-HTTP-Client** program includes `secrets.h` and makes use of the `NET_SSID` and `NET_PASS` constants.  
To make everything work properly create your own `secrets.h` like illustrated below.

```c
#define NET_SSID "<ssid>"
#define NET_PASS "<password>"
```

### Basics

This library provides a simple API to facilitate interaction with [HTTP](https://en.wikipedia.org/wiki/HTTP) web services.  
The `client_connect` function also supports **DNS lookups**.  
A simple HTTP interaction always starts with `client_connect(...)` and ends with `client_disconnect()`.

```c
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  // ... 

  client_disconnect();
```

### API

This library supports **GET** and **POST** requests via `client_get` and `client_post` functions.  
Let's have a look at some examples.

Simple **GET** HTTP request.

```c
  int port = 80;
  char server[] = "arduino-demo.requestcatcher.com";
  client_connect(server, port);

  char path[] = "/";
  client_get(server, path);

  char response[LG_BUFFER];
  if (client_read_response(response, LG_BUFFER))
    Serial.println(response);

  client_disconnect();
```

Simple **POST** HTTP request.

```c
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
```
