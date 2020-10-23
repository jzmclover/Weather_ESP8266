#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "delay.h"
#include "usart.h"

//-----------------OLED端口定义---------------- 

#define OLED_CS     PBout(0)    // CS
#define OLED_DC     PAout(7)    // DC
#define OLED_RST    PAout(6)    // RST
#define OLED_SDA    PAout(5)    // SDA
#define OLED_SCLK   PAout(4)    // SCLK


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_ShowHalfCH(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char BMP[]);
void OLED_Init(void);

#endif  
	 
