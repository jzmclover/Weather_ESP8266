#ifndef __USART_H
#define __USART_H
#include "sys.h"

#define MAX_LENGTH 		1024

void USART1_Init(u32 arr);
void USART2_Init(u32 arr);
void Usart_SendString(char *str, USART_TypeDef* USARTx);


char* USART_GetString(void);

#endif
