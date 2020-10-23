#include "USART.h"
#include "LED.h"
#include "ESP8266.h"
#include "string.h"
#include "delay.h"
#include "oled.h"
#include "Timer.h"
extern int USART1_REC_Finish_FLAG;
extern char Recstring1[20];
extern char ll[8];

int main(void)
{		 
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();
	OLED_Init();
	USART1_Init(460800);
	USART2_Init(460800);
	TIM3_Init(35999,1999); // 1s
	OLED_ShowString(2,20,"OLED Init OK.");
	ESP8266_Init();
	
	
	while(1)
	{
		
	}
}
