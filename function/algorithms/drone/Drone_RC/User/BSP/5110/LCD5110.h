#ifndef _LCD5110_H_
#define _LCD5110_H_
#include "stm32f10x.h"

#include "Font6_8.h"
#include "Font8_16.h"
#include "Icon10_8.h"
#include "Font16_16.h"
//#include "Bitmap.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 
#define u8 		unsigned char
#define u16 	unsigned short

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 




/******************************************************************************
				�����ǻ��������ͳ�ʼ������
*******************************************************************************/ 

/*---��ʼ��5110---*/
void InitLcd(void);

/*---����5110�к�������----*/
void SetXY(u8 row,u8 column);

/*---�������5110��Ļ----*/
void LcdClearAll(void);
	
/******************************************************************************
		�����Ǹ߶�Ϊ8�ĸ��ֲ�������(����6*8�ַ����ַ�����6*8���ţ�10*8ͼ��)
*******************************************************************************/ 

//--every row contains 14 characters,there are 6 rows (font = 6 * 8)--//
//-----------------   	row		  ----------------------//
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 0
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 1
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 2
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 3
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 4
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 5
//------------------------------------------------------//

/*---ֱ��дһ��ASCII�ַ�(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteChar(u8 value);

/*---ֱ��дһ��ASCII�ַ�(6*8)��5110---*/
void PutChar(u8 value,u8 column,u8 row);

/*---ֱ��дһ���ַ���(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteStr(char * str);

/*--ֱ��дһ���ַ���(6*8)��5110--*/
void PutStr(char * str,u8 column,u8 row);

/*---��sign(6*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteSign(char * sign);

/*----��sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row);

/*---��Icon(10*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteIcon(char * icon);

/*--��Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row);


/******************************************************************************
		�����Ǹ߶�Ϊ16�ĸ��ֲ�������(����8*16�ַ����ַ�����16*16����)
*******************************************************************************/ 

//--every row contains 10 characters,there are 3 rows (font = 8 * 16)--//
//-----------       row       ------------//
	//0 1 2 3 ... 81 82 83//	column = 0
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 1
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 2
	//0 1 2 3 ... 81 82 83//
//-----------------------------------------//

/*-----ֱ��дһ��ASCII�ַ�(8*16)��5110------*/
void PutWideChar(u8 value,u8 column,u8 row);

/*--ֱ��дһ���ַ���(6*8)��5110���Զ����У�row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row);

//--every row contains 5 characters,there are 3 rows (font = 16 * 16)--//
//----------      row       --------------//
	//0 1 2 3 ... 81 82 83//	column = 0
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 1
	//0 1 2 3 ... 81 82 83//
	//0 1 2 3 ... 81 82 83//	column = 2
	//0 1 2 3 ... 81 82 83//
//----------------------------------------//

/*-----ֱ��дһ������(16*16)��5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row);

/*--дһ������(16*16),�Զ�����,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num);

/******************************************************************************
		�����ǻ�ͼ�ĸ��ֲ�������(����84*48,�߶�Ϊ8�ı�����ͼƬ)
*******************************************************************************/ 
//---------------   	x		  --------------------//
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 0
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 1
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 2
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 3
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 4
	//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 5
//----------------------------------------------------//

/*---��picture(84*48)��ȫ��---*/
void DrawPicture(char bitmap[]);

/*---��picture(�߶ȱ�����8�ı���)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height);

#endif