/*
 * OLED.h
 *
 *  Created on: 2019��11��4��
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

void OLED_Set_Pos(unsigned char x, unsigned char y);// -- ������ʾ����
void OLED_Fill(unsigned char bmp_dat);// -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
void OLED_CLS(void);// -- ��λ/����
void OLED_Init(void);// -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8������������ʾASCII�����С���У���̫����
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16������������ʾASCII�룬�ǳ�����
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
#if 0
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- ��128x64���ص�BMPλͼ��ȡ������������ֱ���Ȼ���Ƶ�codetab�У��˺������ü���
void OLED_P32x32Ch(unsigned char x,unsigned char y,unsigned int N);
#endif

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowChar(u8 x,u8 y,u8 chr);


#endif /* APP_DRIVER_LIB_INCLUDE_DRIVER_OLED_H_ */