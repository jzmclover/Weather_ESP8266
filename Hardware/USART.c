#include "USART.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"

volatile char receiveBuffer[MAX_LENGTH];

volatile uint16_t receiveLength = 0;
volatile uint8_t rxFlag = 0;

static char str[MAX_LENGTH + 1];
extern int skip;
int times = 0;

int USART1_REC_Finish_FLAG, USART2_REC_Finish_FLAG, num1, num2;
u8 SaveData1[20];
u8 RecData1[20];
char Recstring1[20];

u8 SaveData2[20];
u8 RecData2[20];
char Recstring2[20];

void USART1_Init(u32 arr)
{

	GPIO_InitTypeDef GPIO_InitStructure;     //在GPIO_Init函数调用时需定义一个GPIO_InitTypeDef结构体类型变量
	USART_InitTypeDef USART_InitStructure;   //USART_Init函数调用时需定义一个USART_InitTypeDef结构体类型变量
	NVIC_InitTypeDef NVIC_InitStructure;     //USART_Init函数调用时需定义一个NVIC_InitTypeDef结构体类型变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);	//使能时钟和外设
    
	USART_DeInit(USART1);													//串口1复位

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;    					//PA9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;    		//复用输出
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;    				//PA10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate							= arr;															//波特率
	USART_InitStructure.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;		//模式为接收和发送
	USART_InitStructure.USART_Parity								= USART_Parity_No;									//无奇偶校验
	USART_InitStructure.USART_StopBits							= USART_StopBits_1;									//1个停止位
	USART_InitStructure.USART_WordLength						= USART_WordLength_8b;							//数据长度为8位
	USART_Init(USART1,&USART_InitStructure);			//串口1初始化
	
	NVIC_InitStructure.NVIC_IRQChannel										= USART1_IRQn;								//串口1对应的中断
	NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;											//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;													//抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 1;													//响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//中断初始化
	
	USART_Cmd(USART1, ENABLE); 										//串口1使能
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口1中断
}

void USART2_Init(u32 arr)
{

	GPIO_InitTypeDef GPIO_InitStructure;     //在GPIO_Init函数调用时需定义一个GPIO_InitTypeDef结构体类型变量
	USART_InitTypeDef USART_InitStructure;   //USART_Init函数调用时需定义一个USART_InitTypeDef结构体类型变量
	NVIC_InitTypeDef NVIC_InitStructure;     //USART_Init函数调用时需定义一个NVIC_InitTypeDef结构体类型变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//使能时钟和外设
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);													//串口2复位

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;    					//PA2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;    		//复用输出
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;    					//PA3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate							= arr;															//波特率
	USART_InitStructure.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;		//模式为接收和发送
	USART_InitStructure.USART_Parity								= USART_Parity_No;									//无奇偶校验
	USART_InitStructure.USART_StopBits							= USART_StopBits_1;									//1个停止位
	USART_InitStructure.USART_WordLength						= USART_WordLength_8b;							//数据长度为8位
	USART_Init(USART2,&USART_InitStructure);			//串口2初始化
	
	NVIC_InitStructure.NVIC_IRQChannel										= USART2_IRQn;								//串口2对应的中断
	NVIC_InitStructure.NVIC_IRQChannelCmd									= DISABLE;										//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;													//抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 2;													//响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//中断初始化
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, ENABLE); 										//串口2使能
	delay_my_ms(2000);
	NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;										//使能中断通道
	NVIC_Init(&NVIC_InitStructure);								//中断初始化
}

void Usart_SendString(char *str, USART_TypeDef* USARTx)
{
	while(*str)
	{
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE)); //判断是否可以发送
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); //判断是否发送完成，必须有
		str++;
	}
}

void USART1_IRQHandler(void)        //中断服务函数
{
	int i=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))//收到数据时，该函数返回值为1
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//清除中断接收标志
		if(USART_ReceiveData(USART1)=='\n')	 //检测到新行停止接收
		{ 
			USART1_REC_Finish_FLAG = 1;//完成接收标志位
			for(i=0;i<num1-1;i++)//注意，发送新行发送的是\r\n，因此需要把\r去掉
			{
				SaveData1[i]=RecData1[i];
			}
			sprintf(Recstring1,"%s",SaveData1);//将数组转换为字符串			
			Usart_SendString(Recstring1, USART1);//发送该字符串
			num1=0;					
		}
		else
		{
			if(num1==0)//清空缓冲区
			{
				for(i=0;i<20;i++)
				{
					RecData1[i]='\0';
					SaveData1[i]='\0';
				}
			}	
			RecData1[num1]= USART_ReceiveData(USART1);//将接收字符存入数组
			num1=num1+1;//指针后移
		}			
	}
}

void USART2_IRQHandler(void)
{
	char temp;
 
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		temp = USART_ReceiveData(USART2);
		if(receiveLength == MAX_LENGTH)
		{
			return;
		}
		if(!rxFlag)
		{
			rxFlag = 1;
		}
		if (skip)
		{
			times++;
			if (times > 7500)
			{
				times = 7501;
				receiveBuffer[receiveLength++] = temp;
			}
		}
		else
		{
			receiveBuffer[receiveLength++] = temp;
		}
	}
}

char* USART_GetString(void)
{
	uint16_t temp;
	while(!rxFlag);		//等待接收数据，在串口接收到一帧数据时 rxFlag 将会被置 1 （USART3 中断服务函数中）
	while(rxFlag)
	{
		temp = receiveLength;
		delay_my_ms(10);		//等待 1ms 
		if(temp == receiveLength)	//判断 receiveLength 是否发生变化（USART3 中断函数中 receiveLength 会有变化），如果没有，证明已经收完所有的数据，否则等待接收完成
		{
			rxFlag = 0;
		}
	}
	for(temp = 0; temp < receiveLength; temp++)
	{
		str[temp] = receiveBuffer[temp];
	}
	receiveLength = 0;
	str[temp] = '\0';
	return str;

}
