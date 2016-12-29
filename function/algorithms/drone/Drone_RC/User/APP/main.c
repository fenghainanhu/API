#include "stm32f10x.h"
//#include "stdio.h"//Ϊ������ʹ��printf����ӣ�������ʹ�á�

#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "struct_all.h"

int main(void)
{
	BSP_Int();	//�ײ�������ʼ��
	while(1)
	{
		if(Count_2ms>=1)
		{
			Count_2ms = 0;
			Task_500HZ();
		}
		if(Count_10ms>=5)
		{
			Count_10ms = 0;
			Task_100HZ();
		}
		if(Count_40ms>=20)
		{
			Count_40ms = 0;
			Task_25HZ();
		}
		if(Count_250ms>=125)
		{
			Count_250ms = 0;
			Task_4HZ();
		}
	}
}

//int fputc(int ch, FILE *f)	//Ϊ������ʹ��printf����ӣ�������ʹ�á�
//{
//	USART_SendData(USART1, (u8) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}
