#include <stdlib.h>
#include "utils.h"

/**
 * Produce a random floating point number from min to max.
 * Rand must be correctly seeded before usage: "srand(millis());"
 * 
 * @param min - The server IP address or domain name.
 * @param max - The server port.
 * @return - The resulting number.
 */
float rand_float(float min, float max)
{
    return min + ((float) rand() / (float) RAND_MAX) * (max - min);
}
