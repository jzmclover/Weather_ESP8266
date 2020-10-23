#ifndef __ESP8266_H
#define __ESP8266_H

#define WIFI_RST PAout(1)
void ESP8266_Init(void);
void Showweathername(char* r);
void ShowWind(float m, float n);
char* Getweatherinfo(void);
void Cleartemp(void);
#endif
