typedef struct {
  float lat, lon;
} gpsData;

void d(char* s);
int parseGPGGA(char* gpggaString, gpsData* dataStore);