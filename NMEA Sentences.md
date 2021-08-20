# GPS Data
## GPGGA
### Latitude and Longitude
- Measured in degrees
- Minutes and seconds are like a clock, 1 minute is 1/60th of a degree, 1 second is 1/60th of a minute
- Latitude is N/S, longitude is E/W from the Meridian
```
0: $GPGGA,
1: hhmmss.ss,       - UTC time
2: ddmm.mmmmm,      - latitude
3: N/S,             - latitude is north or south
4: dddmm.mmmmm,      - longitude of position
5: E/W,             - longitude is east or west
6: x,               - GPS fix indicator (1 if there is a fix)
7: xx,              - Number of satellites used
8: x.x,             - Horizontal dilution of precision
9: x.x,             - Altitude
10: M,              - Units of altitude (meters)
11: x.x,            - Geoidal separation
12: M,              - Units of geoidal separation
13: *cc             - Checksum
```