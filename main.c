#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEBUG 1
#define BUF_LEN 100
#define TIME_LEN 10
#define GPGGA_STR "$GPGGA"

/*
Debug printing function
*/
void d(char* s) {
  if (DEBUG) {
    printf("DEBUG: ");
    printf(s);
    printf("\n");
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Usage: gpsreader <serial port>\n");
    return 0;
  }
  d(argv[1]);

  int fd = open(argv[1], O_RDWR | O_NOCTTY);
  char readBuf[BUF_LEN];
  int readLen;

  for (;;) {
    readLen = read(fd, readBuf, BUF_LEN);
    readBuf[readLen] = 0; //Null terminate
    //d(readBuf);
    if (strstr(readBuf, GPGGA_STR)) {
      d(readBuf);
      //When we hit a GPGGA packet, tokenise data
      char *tok = strtok(readBuf, ",");
      //Make 100% sure we are at the start of the GPGGA sentence
      if (strstr(tok, GPGGA_STR)) {
        tok = strtok(NULL, ",");
        char time[10];
        time[10] = 0;
        strcpy(time, tok);
        d(time);
      }
    }
  }
}