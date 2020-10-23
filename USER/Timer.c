#include "stm32f10x.h"
#include "Timer.h"
#include "LED.h"

static int count=0;
int refresh = 0;

void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
  //TIM3����������������
  //Ƶ��=TIM1_CLK/(ARR+1)/(psc+1)
	TIM_TimeBaseStructure.TIM_Period = arr; //�������ڣ����ϼǴ�������������0	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //��ʱ����Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��TIM3ָ���Ļ�׼��λ
 
	//�ж����ȼ�NVIC����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3			 
}

void TIM3_IRQHandler(void)   //TIM3�жϷ������
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�ж��Ƿ���
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIM3��־λ
		count++;
		if (count >= 600) // ÿ10���Ӹ���1��
		{
			count = 0;
			LED1 = ~LED1;
			refresh = 1;
		}
	}
}
