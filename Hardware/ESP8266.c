#include "ESP8266.h"
#include "USART.h"
#include "string.h"
#include "LED.h"
#include "delay.h"
#include "oled.h"
#include "stdlib.h"
#include "stdio.h"

char* temp = NULL;
char* p = NULL;
char* q = NULL;

char weather_code [20];  // ��������
char temperature [5];    // �¶�
char humidity [5];       // ʪ��
char pressure [10];      // ��ѹ
char apparent_temperature [5];  // ����¶�
char pm25 [5];  // PM2.5
char aqi [5];   // AQI
char speed [7]; // ����
char direction [7]; //����

int i=0;
int skip = 0;
int page = 0;
float m,n;
extern int refresh;

void ESP8266_Init()
{
	WIFI_RST = 0;
	delay_my_ms(10);
	WIFI_RST = 1;
	delay_my_ms(6000);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�ж�
	Usart_SendString("+++", USART2);
	delay_my_ms(400);
	Usart_SendString("AT\r\n", USART2); //����
	delay_my_ms(400);

	OLED_ShowString(0,0,"Connecting");
	OLED_ShowString(0,20,">             ");
	
	Usart_SendString(temp, USART1);
	Usart_SendString("AT+CWMODE=3\r\n", USART2);
	delay_my_ms(400);
	Usart_SendString(USART_GetString(), USART1);
	
	OLED_ShowString(0,0,"Connecting.");
	OLED_ShowString(0,20,">>>>          ");
	
//	Usart_SendString("AT+CWJAP=\"549\",\"cn-csgao549\"\r\n", USART2);
//	delay_my_ms(3000);
//	delay_my_ms(3000);
//	delay_my_ms(3000);
//	temp = USART_GetString(); 
//	Usart_SendString(temp, USART1);
	
	Usart_SendString("AT+CIPSTART=\"TCP\",\"api.caiyunapp.com\",80\r\n", USART2);
	delay_my_ms(1000);
	Usart_SendString(USART_GetString(), USART1);
	OLED_ShowString(0,0,"Connecting..");
	OLED_ShowString(0,20,">>>>>>>       ");
	
	Usart_SendString("AT+CIPMODE=1\r\n", USART2);
	delay_my_ms(400);
	Usart_SendString(USART_GetString(), USART1);
	OLED_ShowString(0,0,"Connecting...");
	OLED_ShowString(0,20,">>>>>>>>>>    ");
	
	Usart_SendString("AT+CIPSEND\r\n", USART2);
	delay_my_ms(400);
	Usart_SendString(USART_GetString(), USART1);
	OLED_ShowString(0,0,"Connecting....");
	OLED_ShowString(0,20,">>>>>>>>>>>>> ");
	
	// ��ȡ������Ϣ
	temp = Getweatherinfo();
	Usart_SendString(temp, USART1);
	OLED_Clear();
	OLED_ShowString(20,20,"Updating...");
	// ��������
	p = strstr(temp,"skycon");
	q = strstr(p,"\",\"");
	strncpy(weather_code,p+9,q-p-9); // weather_code��ʾ��������
	
	// �¶�
	p = strstr(temp,"temperature");
	q = strstr(p,".");
	strncpy(temperature,p+13,q-p-13); // temperature��ʾ�¶�
	
	// ʪ��
	p = strstr(temp,"humidity");
	q = strstr(p,",");
	strncpy(humidity,p+12,q-p-12); // humidity��ʾʪ��
	if (q-p-12==1)
	{
		humidity[1] = '0';
		humidity[2] = '\0';
	}
	
	// ��ѹ
	p = strstr(temp,"pressure");
	q = strstr(p,".");
	strncpy(pressure,p+10,q-p-10); // pressure��ʾ��ѹ
	
	// ����¶�
	p = strstr(temp,"apparent_temperature");
	q = strstr(p,",");
	strncpy(apparent_temperature,p+22,q-p-22); // apparent_temperature��ʾ����¶�
	
	// PM2.5
	p = strstr(temp,"pm25");
	q = strstr(p,".");
	strncpy(pm25,p+6,q-p-6); // pm25��ʾPM2.5
	
	// AQI
	p = strstr(temp,"aqi");
	q = strstr(p,".");
	strncpy(aqi,p+12,q-p-12); // aqi��ʾAQI
	
	// ����
	p = strstr(temp,"speed");
	q = strstr(p,",");
	strncpy(speed,p+7,q-p-7); // speed��ʾ����
	m = atoi(speed);
	
	// ����
	p = strstr(temp,"direction");
	q = strstr(p,"}");
	strncpy(direction,p+11,q-p-11); // direction��ʾ����
	n = atoi(direction);
	
		while(1)
	{
		// ��ȡ������Ϣ
		if (refresh == 1)
		{
			refresh = 0;
			// ��ȡ������Ϣ
			page = 0;
			temp = Getweatherinfo();
			Cleartemp();
			OLED_Clear();
			OLED_ShowString(20,20,"Updating...");
			// ��������
			p = strstr(temp,"skycon");
			q = strstr(p,"\",\"");
			strncpy(weather_code,p+9,q-p-9); // weather_code��ʾ��������
			
			// �¶�
			p = strstr(temp,"temperature");
			q = strstr(p,".");
			strncpy(temperature,p+13,q-p-13); // temperature��ʾ�¶�
			
			// ʪ��
			p = strstr(temp,"humidity");
			q = strstr(p,",");
			strncpy(humidity,p+12,q-p-12); // humidity��ʾʪ��
			if (q-p-12==1)
			{
				humidity[1] = '0';
				humidity[2] = '\0';
			}
			
			// ��ѹ
			p = strstr(temp,"pressure");
			q = strstr(p,".");
			strncpy(pressure,p+10,q-p-10); // pressure��ʾ��ѹ
			
			// ����¶�
			p = strstr(temp,"apparent_temperature");
			q = strstr(p,",");
			strncpy(apparent_temperature,p+22,q-p-22); // apparent_temperature��ʾ����¶�
			
			// PM2.5
			p = strstr(temp,"pm25");
			q = strstr(p,".");
			strncpy(pm25,p+6,q-p-6); // pm25��ʾPM2.5
			
			// AQI
			p = strstr(temp,"aqi");
			q = strstr(p,".");
			strncpy(aqi,p+12,q-p-12); // aqi��ʾAQI
			
			// ����
			p = strstr(temp,"speed");
			q = strstr(p,",");
			strncpy(speed,p+7,q-p-7); // speed��ʾ����
			m = atoi(speed);
			
			// ����
			p = strstr(temp,"direction");
			q = strstr(p,"}");
			strncpy(direction,p+11,q-p-11); // direction��ʾ����
			n = atoi(direction);
		}
		if (page>=0 && page<8)
		{ 
			if (page == 0)
			{
				OLED_Clear();
			}
			//��0ҳ��ʾ���������¶ȡ����򡢷���
			page = page + 1;
			OLED_ShowCHinese(0+16*0,0,27);  //��
			OLED_ShowCHinese(0+16*1,0,20);  //ǰ
			OLED_ShowCHinese(0+16*2,0,24);  //��
			OLED_ShowCHinese(0+16*3,0,29);  //��
			
			OLED_ShowCHinese(0+16*5,0,31);  //��
			OLED_ShowCHinese(0+16*6,0,6);  //��
			
			Showweathername(weather_code);
			if (temperature[0] == '-')
			{
				OLED_ShowString(0+16*5-8,20,temperature);
			}
			else
			{
				OLED_ShowString(0+16*5,20,temperature);
			}
			OLED_ShowCHinese(0+16*6,20,33); //��
			
			ShowWind(n,m);
		}
		else if (page>=8 && page <16)
		{ 
			if (page == 8)
			{
				OLED_Clear();
			}
			//��1ҳ��ʾ��ʪ�ȡ���ѹ������¶�
			page = page + 1;
			
			OLED_ShowCHinese(0+16*1,0,32);  //ʪ
			OLED_ShowCHinese(0+16*2,0, 6);  //��
			
			OLED_ShowCHinese(0+16*1,20,29);  //��
			OLED_ShowCHinese(0+16*2,20,23);  //ѹ
			
			OLED_ShowString(0+16*5,0,humidity);
			OLED_ShowChar(0+16*6,0,'%',16,1);
			OLED_ShowString(0+16*4-4,20,pressure);
			OLED_ShowString(0+16*7,20,"Pa"); //Pa
			
			OLED_ShowCHinese(0+16*1,40,19);  //��
			OLED_ShowCHinese(0+16*2,40,28);  //��
			
			if (apparent_temperature[0] == '-')
			{
				OLED_ShowString(0+16*4-4,40,apparent_temperature);
			}
			else
			{
				OLED_ShowString(0+16*4+4,40,apparent_temperature);
			}
			
			OLED_ShowCHinese(0+16*6+4,40,33); //��
		}
		else if(page>=16)
		{
			if (page == 16)
			{
				OLED_Clear();
			}
			page = page + 1;
			//��2ҳ��ʾ��PM2.5��AQI������
			OLED_ShowString(0+16*1,0,"PM2.5");
			OLED_ShowString(0+16*5+4,0,"AQI");
			
			OLED_ShowString(0+16*2-4,20,pm25);
			OLED_ShowString(0+16*5+8,20,aqi);
			
			OLED_ShowCHinese(0+16*1,40,24);  //��
			OLED_ShowCHinese(0+16*2,40,30);  //��
			OLED_ShowCHinese(0+16*5,40,22);  //��
			OLED_ShowCHinese(0+16*6,40,26);  //��
			if (page==22)
			{
				page = 0;
			}
		}
		
		delay_my_ms(1000);
		LED1 = ~LED1;
	}

}

char* Getweatherinfo()
{
	char* temp1 = NULL;
	Usart_SendString("GET /v2.5/TAkhjf8d1nlSlspN/117.15074,39.1382/realtime.json HTTP/1.1\r\n\
Host: api.caiyunapp.com\r\n\
Connection: keep-alive\r\n\
Pragma: no-cache\r\n\
Cache-Control: no-cache\r\n\
DNT: 1\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36\r\n\r\n", USART2);
	delay_my_ms(1000);
	temp1 = USART_GetString();
	return temp1;
}

void Cleartemp()
{
	for (i=0;i<20;i++)
	{
		weather_code[i] = '\0';
	}
	for (i=0;i<5;i++)
	{
		humidity[i] = '\0';
		pm25[i] = '\0';
		aqi[i] = '\0';
		temperature[i] = '\0';
		apparent_temperature[i] = '\0';
	}
	for (i=0;i<10;i++)
	{
		pressure[i] = '\0';
	}
	for (i=0;i<7;i++)
	{
		speed[i] = '\0';
		direction[i] = '\0';
	}
}

void Showweathername(char* r)
{
	if (strcmp(r,"CLEAR_DAY")==0 || strcmp(r,"CLEAR_NIGHT")==0)
	{
		OLED_ShowCHinese(0+16*1+8,20,7);  //��
	}
	if (strcmp(r,"PARTLY_CLOUDY_DAY")==0 || strcmp(r,"PARTLY_CLOUDY_NIGHT")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 2);  //��
		OLED_ShowCHinese(0+16*2,20, 1);  //��
	}
	if (strcmp(r,"CLOUDY")==0)
	{
		OLED_ShowCHinese(0+16*1+8,20,13);  //��
	}
	if (strcmp(r,"LIGHT_HAZE")==0)
	{
		OLED_ShowCHinese(0+16*0,20,11);  //��
		OLED_ShowCHinese(0+16*1,20, 6);  //��
		OLED_ShowCHinese(0+16*2,20,16);  //��
		OLED_ShowCHinese(0+16*3,20,17);  //��
	}
	if (strcmp(r,"MODERATE_HAZE")==0)
	{
		OLED_ShowCHinese(0+16*0,20, 0);  //��
		OLED_ShowCHinese(0+16*1,20, 6);  //��
		OLED_ShowCHinese(0+16*2,20,16);  //��
		OLED_ShowCHinese(0+16*3,20,17);  //��
	}
	if (strcmp(r,"HEAVY_HAZE")==0)
	{
		OLED_ShowCHinese(0+16*0,20,12);  //��
		OLED_ShowCHinese(0+16*1,20, 6);  //��
		OLED_ShowCHinese(0+16*2,20,16);  //��
		OLED_ShowCHinese(0+16*3,20,17);  //��
	}
	if (strcmp(r,"LIGHT_RAIN")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 4);  //С
		OLED_ShowCHinese(0+16*2,20,14);  //��
	}
	if (strcmp(r,"MODERATE_RAIN")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 0);  //��
		OLED_ShowCHinese(0+16*2,20,14);  //��
	}
	if (strcmp(r,"HEAVY_RAIN")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 3);  //��
		OLED_ShowCHinese(0+16*2,20,14);  //��
	}
	if (strcmp(r,"STORM_RAIN")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 8);  //��
		OLED_ShowCHinese(0+16*2,20,14);  //��
	}
	if (strcmp(r,"FOG")==0)
	{
		OLED_ShowCHinese(0+16*1+8,20,16);  //��
	}
	if (strcmp(r,"LIGHT_SNOW")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 4);  //С
		OLED_ShowCHinese(0+16*2,20,15);  //ѩ
	}
	if (strcmp(r,"MODERATE_SNOW")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 0);  //��
		OLED_ShowCHinese(0+16*2,20,15);  //ѩ
	}
	if (strcmp(r,"HEAVY_SNOW")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 3);  //��
		OLED_ShowCHinese(0+16*2,20,15);  //ѩ
	}
	if (strcmp(r,"STORM_SNOW")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 8);  //��
		OLED_ShowCHinese(0+16*2,20,15);  //ѩ
	}
	if (strcmp(r,"DUST")==0)
	{
		OLED_ShowCHinese(0+16*1,20,10);  //��
		OLED_ShowCHinese(0+16*2,20, 5);  //��
	}
	if (strcmp(r,"SAND")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 9);  //ɳ
		OLED_ShowCHinese(0+16*2,20, 5);  //��
	}
	if (strcmp(r,"WIND")==0)
	{
		OLED_ShowCHinese(0+16*1,20, 3);  //��
		OLED_ShowCHinese(0+16*2,20,18);  //��
	}
}

void ShowWind(float m, float n)
{
	if (m<22.5 || m>=337.5)
	{
		OLED_ShowCHinese(0+16*1,40,37); // ��
		OLED_ShowCHinese(0+16*2,40,18); // ��
	}
	else if (m>=22.5 && m<67.5)
	{
		OLED_ShowCHinese(0+16*0+8,40,35); // ��
		OLED_ShowCHinese(0+16*1+8,40,37); // ��
		OLED_ShowCHinese(0+16*2+8,40,18); // ��
	}
	else if (m>=67.5 && m<112.5)
	{
		OLED_ShowCHinese(0+16*1,40,35); // ��
		OLED_ShowCHinese(0+16*2,40,18); // ��
	}
	else if (m>=112.5 && m<157.5)
	{
		OLED_ShowCHinese(0+16*0+8,40,35); // ��
		OLED_ShowCHinese(0+16*1+8,40,22); // ��
		OLED_ShowCHinese(0+16*2+8,40,18); // ��
	}
	else if (m>=157.5 && m<202.5)
	{
		OLED_ShowCHinese(0+16*1,40,22); // ��
		OLED_ShowCHinese(0+16*2,40,18); // ��
	}
	else if (m>=202.5 && m<247.5)
	{
		OLED_ShowCHinese(0+16*0+8,40,36); // ��
		OLED_ShowCHinese(0+16*1+8,40,22); // ��
		OLED_ShowCHinese(0+16*2+8,40,18); // ��
	}
	else if (m>=247.5 && m<295.5)
	{
		OLED_ShowCHinese(0+16*1,40,36); // ��
		OLED_ShowCHinese(0+16*2,40,18); // ��
	}
	else if (m>=295.5 && m<337.5)
	{
		OLED_ShowCHinese(0+16*0+8,40,36); // ��
		OLED_ShowCHinese(0+16*1+8,40,37); // ��
		OLED_ShowCHinese(0+16*2+8,40,18); // ��
	}
	
	if (n<1)
	{
		OLED_ShowNumber(0+16*5,40,0,2,16); // 0
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=1 && n<5.5)
	{
		OLED_ShowNumber(0+16*5,40,1,2,16); // 1
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=5.5 && n<11.5)
	{
		OLED_ShowNumber(0+16*5,40,2,2,16); // 2
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=11.5 && n<19.5)
	{
		OLED_ShowNumber(0+16*5,40,3,2,16); // 3
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=19.5 && n<28.5)
	{
		OLED_ShowNumber(0+16*5,40,4,2,16); // 4
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=28.5 && n<38.5)
	{
		OLED_ShowNumber(0+16*5,40,5,2,16); // 5
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=38.5 && n<49.5)
	{
		OLED_ShowNumber(0+16*5,40,6,2,16); // 6
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=49.5 && n<61.5)
	{
		OLED_ShowNumber(0+16*5,40,7,2,16); // 7
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=61.5 && n<74.5)
	{
		OLED_ShowNumber(0+16*5,40,8,2,16); // 8
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=74.5 && n<88.5)
	{
		OLED_ShowNumber(0+16*5,40,9,2,16); // 9
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=88.5 && n<102.5)
	{
		OLED_ShowNumber(0+16*5,40,10,2,16); // 10
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=102.5 && n<117.5)
	{
		OLED_ShowNumber(0+16*5,40,11,2,16); // 11
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=117.5 && n<133.5)
	{
		OLED_ShowNumber(0+16*5,40,12,2,16); // 12
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=133.5 && n<149.5)
	{
		OLED_ShowNumber(0+16*5,40,13,2,16); // 13
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=149.5 && n<166.5)
	{
		OLED_ShowNumber(0+16*5,40,14,2,16); // 14
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=166.5 && n<183.5)
	{
		OLED_ShowNumber(0+16*5,40,15,2,16); // 15
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=183.5 && n<201.5)
	{
		OLED_ShowNumber(0+16*5,40,16,2,16); // 16
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
	else if (n>=201.5 && n<220)
	{
		OLED_ShowNumber(0+16*5,40,17,2,16); // 17
		OLED_ShowCHinese(0+16*6,40,34); // ��
	}
}
