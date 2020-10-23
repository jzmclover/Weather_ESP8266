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
	SystemInit();	// 配置系统时钟为72M 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
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
