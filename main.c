#include "main.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define GPGGA_STR "$GPGGA"

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Usage: nmeaparser <serial port>\n");
    return 0;
  }

  int fd = open(argv[1], O_RDWR | O_NOCTTY);
  char readBuf[BUF_LEN];
  int readLen;

  position pos;

  for (;;) {
    readLen = read(fd, readBuf, BUF_LEN);
    readBuf[readLen] = '\0'; //Null terminate!

    if (strstr(readBuf, GPGGA_STR)) {
      if (parseGPGGA(readBuf, &pos) == SUCCESS) { //If we have a successful parse of a GPGGA packet
        //Example can open an OSM link to your position
        char url[150];
        char command[150];
        sprintf(url, "https://www.openstreetmap.org/search?whereami=1&query=%f%%2C%f\n", pos.lat, pos.lon);
        sprintf(command, "exo-open \"%s\" 0> /dev/null", url);
        close(fd);

        printf("Position: %f,%f.\nOpen in browser (with exo-open)? [y/n]\n", pos.lat, pos.lon);
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y' || choice == 0x0d) {
          system(command);
        }
        return(0);
      }
    }
  }
}

/*
Parse a string containing a GPGGA packet
*/
int parseGPGGA(char* gpggaString, position* pos) {
  if (DEBUG) printf("Sentence: %s", gpggaString);

  //When we hit a GPGGA packet, tokenise gpsData
  char *tok = strtok(gpggaString, ",");
  if (strstr(tok, GPGGA_STR)) {
    //Get time
    tok = strtok(NULL, ",");
    if(DEBUG) printf("Time: %s\n", tok);

    //Get latitude data
    tok = strtok(NULL, ",");
    char lat[15];
    strcpy(lat, tok);
    tok = strtok(NULL, ",");
    bool N;
    if (strstr(tok, "N")) N = 1;
    else N = 0;
    double dLat = parseLatLong(strtod(lat, NULL), N);
    if(DEBUG) printf("Latitude: %f\n", dLat);

    //Get longitude data
    tok = strtok(NULL, ",");
    char lon[15];
    strcpy(lon, tok);
    tok = strtok(NULL, ",");
    bool E;
    if (strstr(tok, "E")) E = 1;
    else E = 0;
    double dLon = parseLatLong(strtod(lon, NULL), E);
    if(DEBUG) printf("Longitude: %f\n", dLon);

    pos->lat = dLat;
    pos->lon = dLon;

    return SUCCESS;
  }
}

/*
Convert latitude or longitude in form (d)ddmm.mmmmm to format ddd.ddddd as a float
*/
double parseLatLong(double latLong, bool isPositive) {
  int degrees = latLong / 100.0;
  double minutes = fmod(latLong, 100.0);
  return isPositive ? (degrees + (minutes / 60.0)) : -(degrees + (minutes / 60.0));
}