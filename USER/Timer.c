#include "stm32f10x.h"
#include "Timer.h"
#include "LED.h"

static int count=0;
int refresh = 0;

void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
  //TIM3基本计数器设设置
  //频率=TIM1_CLK/(ARR+1)/(psc+1)
	TIM_TimeBaseStructure.TIM_Period = arr; //计数周期，向上记此数，计数器清0	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //定时器分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使用TIM3指定的基准单位
 
	//中断优先级NVIC设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置中断分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3			 
}

void TIM3_IRQHandler(void)   //TIM3中断服务程序
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检测TIM3中断是否发生
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM3标志位
		count++;
		if (count >= 600) // 每10分钟更新1次
		{
			count = 0;
			LED1 = ~LED1;
			refresh = 1;
		}
	}
}
