#include "stm32f10x.h"
//#include "stdio.h"//Ϊ������ʹ��printf����ӣ�������ʹ�á�

#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "struct_all.h"

int main(void)
{
	BSP_Int();	//�ײ�������ʼ��
	while(1)
	{
		Task();
	}
}

//int fputc(int ch, FILE *f)	//Ϊ������ʹ��printf����ӣ�������ʹ�á�
//{
//	USART_SendData(USART1, (uint8_t) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}
