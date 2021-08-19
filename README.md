# NMEAParser
- Simple C program to handle NMEA sentences from a serial GPS module
- To build (with debugging messages): `gcc main.c -DDEBUG=1 -lm -o nmeaparser`
- To run: `nmeaparser /dev/tty*` depending on which tty the GPS is on