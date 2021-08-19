#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define BUF_LEN 100
#define TIME_LEN 10

#define SUCCESS 0
#define FAIL 1

typedef unsigned char bool;

typedef struct {
  double lat, lon;
} position;

int parseGPGGA(char* gpggaString, position* dataStore);
double parseLatLong(double latLong, bool isPositive);