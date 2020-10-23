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

	GPIO_InitTypeDef GPIO_InitStructure;     //��GPIO_Init��������ʱ�趨��һ��GPIO_InitTypeDef�ṹ�����ͱ���
	USART_InitTypeDef USART_InitStructure;   //USART_Init��������ʱ�趨��һ��USART_InitTypeDef�ṹ�����ͱ���
	NVIC_InitTypeDef NVIC_InitStructure;     //USART_Init��������ʱ�趨��һ��NVIC_InitTypeDef�ṹ�����ͱ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);	//ʹ��ʱ�Ӻ�����
    
	USART_DeInit(USART1);													//����1��λ

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;    					//PA9
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;    		//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;    				//PA10
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate							= arr;															//������
	USART_InitStructure.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;		//ģʽΪ���պͷ���
	USART_InitStructure.USART_Parity								= USART_Parity_No;									//����żУ��
	USART_InitStructure.USART_StopBits							= USART_StopBits_1;									//1��ֹͣλ
	USART_InitStructure.USART_WordLength						= USART_WordLength_8b;							//���ݳ���Ϊ8λ
	USART_Init(USART1,&USART_InitStructure);			//����1��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel										= USART1_IRQn;								//����1��Ӧ���ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;											//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;													//��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 1;													//��Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//�жϳ�ʼ��
	
	USART_Cmd(USART1, ENABLE); 										//����1ʹ��
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������1�ж�
}

void USART2_Init(u32 arr)
{

	GPIO_InitTypeDef GPIO_InitStructure;     //��GPIO_Init��������ʱ�趨��һ��GPIO_InitTypeDef�ṹ�����ͱ���
	USART_InitTypeDef USART_InitStructure;   //USART_Init��������ʱ�趨��һ��USART_InitTypeDef�ṹ�����ͱ���
	NVIC_InitTypeDef NVIC_InitStructure;     //USART_Init��������ʱ�趨��һ��NVIC_InitTypeDef�ṹ�����ͱ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��ʱ�Ӻ�����
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);													//����2��λ

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;    					//PA2
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;    		//�������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;    					//PA3
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate							= arr;															//������
	USART_InitStructure.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;		//ģʽΪ���պͷ���
	USART_InitStructure.USART_Parity								= USART_Parity_No;									//����żУ��
	USART_InitStructure.USART_StopBits							= USART_StopBits_1;									//1��ֹͣλ
	USART_InitStructure.USART_WordLength						= USART_WordLength_8b;							//���ݳ���Ϊ8λ
	USART_Init(USART2,&USART_InitStructure);			//����2��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel										= USART2_IRQn;								//����2��Ӧ���ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd									= DISABLE;										//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;													//��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 2;													//��Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//�жϳ�ʼ��
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, ENABLE); 										//����2ʹ��
	delay_my_ms(2000);
	NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;										//ʹ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								//�жϳ�ʼ��
}

void Usart_SendString(char *str, USART_TypeDef* USARTx)
{
	while(*str)
	{
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE)); //�ж��Ƿ���Է���
		USART_SendData(USARTx,*str);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); //�ж��Ƿ�����ɣ�������
		str++;
	}
}

void USART1_IRQHandler(void)        //�жϷ�����
{
	int i=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))//�յ�����ʱ���ú�������ֵΪ1
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//����жϽ��ձ�־
		if(USART_ReceiveData(USART1)=='\n')	 //��⵽����ֹͣ����
		{ 
			USART1_REC_Finish_FLAG = 1;//��ɽ��ձ�־λ
			for(i=0;i<num1-1;i++)//ע�⣬�������з��͵���\r\n�������Ҫ��\rȥ��
			{
				SaveData1[i]=RecData1[i];
			}
			sprintf(Recstring1,"%s",SaveData1);//������ת��Ϊ�ַ���			
			Usart_SendString(Recstring1, USART1);//���͸��ַ���
			num1=0;					
		}
		else
		{
			if(num1==0)//��ջ�����
			{
				for(i=0;i<20;i++)
				{
					RecData1[i]='\0';
					SaveData1[i]='\0';
				}
			}	
			RecData1[num1]= USART_ReceiveData(USART1);//�������ַ���������
			num1=num1+1;//ָ�����
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
	while(!rxFlag);		//�ȴ��������ݣ��ڴ��ڽ��յ�һ֡����ʱ rxFlag ���ᱻ�� 1 ��USART3 �жϷ������У�
	while(rxFlag)
	{
		temp = receiveLength;
		delay_my_ms(10);		//�ȴ� 1ms 
		if(temp == receiveLength)	//�ж� receiveLength �Ƿ����仯��USART3 �жϺ����� receiveLength ���б仯�������û�У�֤���Ѿ��������е����ݣ�����ȴ��������
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
