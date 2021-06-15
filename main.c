#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"

#define DEBUG 1
#define BUF_LEN 100
#define TIME_LEN 10
#define GPGGA_STR "$GPGGA"

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Usage: gpsreader <serial port>\n");
    return 0;
  }

  int fd = open(argv[1], O_RDWR | O_NOCTTY);
  char readBuf[BUF_LEN];
  int readLen;

  for (;;) {
    readLen = read(fd, readBuf, BUF_LEN);
    readBuf[readLen] = '\0'; //Null terminate

    gpsData* gpggaData;

    if (strstr(readBuf, GPGGA_STR)) {
      if (parseGPGGA(readBuf, gpggaData)) { //If we have a successful parse
        //TODO: Print info here
      }
    }
  }
}

int parseGPGGA(char* gpggaString, gpsData* dataStore) {
  if (DEBUG) printf("Sentence: %s", gpggaString);

  //When we hit a GPGGA packet, tokenise gpsData
  char *tok = strtok(gpggaString, ",");
  if (strstr(tok, GPGGA_STR)) {
    //Get time
    tok = strtok(NULL, ",");
    char time[10];
    time[sizeof(time) - 1] = '\0';
    strcpy(time, tok);
    if(DEBUG) printf("Time: %s\n", time);

    //Get position data
    tok = strtok(NULL, ",");
    //TODO: Handle position data
  }
}