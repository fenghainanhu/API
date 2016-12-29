#include "struct_all.h"
#include "Filter.h"
#define high_max 2000
#define high_min -100
#define d_max  20

float high=0,Filter_high=0,old_high=0,d_high=0;
uint8_t high_f=0,high_ok=0,high_err=0,high_err_temp=0;

uint8_t	US_Offset = 1;//�Զ�У��
uint16_t offset_high=0;

void get_high(void)
{
	high_f=1;
	high_err_temp++;
	USART3_PrintHexU8(0x55);
}
void receive_higt(uint8_t com_data)
{
	static uint16_t high_temp;
	
	if(high_f == 1)
	{
		high_temp = com_data;
		high_f = 2;
	}
	else if(high_f == 2)
	{
		high = ((high_temp<<8) | com_data ) - offset_high;
	
		if( high<=high_min ) high = high_min; //�޷�
		if( high>=high_max ) high = high_max; //�޷�
		
		high_err=high_err_temp;
		high_err_temp = 0;
		if(high_err > 1 && Filter_high <= 200) high = 0;//�������������̫��
			
		//US_100_Offset();//��ƫУ׼
		US_100_Filter(&high,&Filter_high);//�����˲�
		
		d_high = Filter_high - old_high;
		old_high = Filter_high;
		
		high_f = 0;
		high_ok= 1;
	}
}

/******************************************************************************
����ԭ�ͣ�	void US_100_Offset(void)
��    �ܣ�	��������ƫУ��
*******************************************************************************/ 
void US_100_Offset(void)
{
	static int32_t high_sum;
	static uint8_t count_high=0;
	
	if(US_Offset)
	{
		if(count_high==0)
		{
			offset_high = 0;
			high_sum = 0;
			count_high = 1;
			return;
		}
		else
		{
			count_high++;
			high_sum += high;
		}
		if(count_high==11)
		{
			count_high--;
			offset_high = high_sum / count_high;
			count_high = 0;
			US_Offset = 0;
		}
	}
}

void Do_US_Offset(void)
{
	US_Offset=1;
}

