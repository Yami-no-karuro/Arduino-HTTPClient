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

The main program includes `secrets.h` and makes use of the `NET_SSID` and `NET_PASS` constants.  
To make everything work properly create your own `secrets.h` like illustrated below.

```c
#define NET_SSID "<ssid>"
#define NET_PASS "<password>"
```
