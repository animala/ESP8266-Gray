/*
 * OLED.h
 *
 *  Created on: 2019年11月4日
 *      Author: 10224
 */

#ifndef APP_DRIVER_LIB_INCLUDE_DRIVER_OLED_H_
#define APP_DRIVER_LIB_INCLUDE_DRIVER_OLED_H_

#include "eagle_soc.h"
#include "c_types.h"

#define high 1
#define low 0
#define OLED_MODE 0
#define SIZE 1
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED_Set_Pos(unsigned char x, unsigned char y);// -- 设置显示坐标
void OLED_Fill(unsigned char bmp_dat);// -- 全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS(void);// -- 复位/清屏
void OLED_Init(void);// -- OLED屏初始化程序，此函数应在操作屏幕之前最先调用
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16点整，用于显示ASCII码，非常清晰
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
#if 0
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
void OLED_P32x32Ch(unsigned char x,unsigned char y,unsigned int N);
#endif

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowChar(u8 x,u8 y,u8 chr);


#endif /* APP_DRIVER_LIB_INCLUDE_DRIVER_OLED_H_ */
