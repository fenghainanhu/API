#include "Tasks.h"
#include "Filter.h"

/******************************************************************************
����ԭ��:	static void Nvic_Init(void)
��������:	NVIC��ʼ��
*******************************************************************************/ 
static void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
	//Timer3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//Nrf2401�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//MODE����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//FUN����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
����ԭ�ͣ�	void BSP_Int(void)
��    �ܣ�	Ӳ��������ʼ��
*******************************************************************************/ 
void BSP_Int(void)
{
	Uart1_Init(115200);	//���ڳ�ʼ��
	LED_Init();//LED��ʼ��
	KEY_Init();//������ʼ��
	Timer3_Init(500);//Timer3�ж�500HZ
	Nvic_Init();//�ж����ȼ���ʼ��	
	ADC1_Init();//ADC��DMA��ʼ��
	SPI2_Init();//SPI2��ʼ��
	NRF24L01_Init(35,TX);//2401ѡ��40ͨ��������ģʽ
 	NRF24L01_Check();//���2401�Ƿ�����
	LED_ON_OFF();//LED��˸
	
	PrintString("\r\n RagingFire_RC V2.2.2 \r\n");//�汾��
	Bsp_Int_Ok = 1;
}

/******************************************************************************
����ԭ�ͣ�	void Task_500HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ500HZ����
*******************************************************************************/ 
void Task_500HZ(void)
{
	Nrf_Connect();//NRF24L01���Ӻ���
	Rc_Filter(&Rc,&Filter_Rc);//����ң��ָ��+�˲�
}

/******************************************************************************
����ԭ�ͣ�	void Task_100HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ100HZ����
*******************************************************************************/ 
void Task_100HZ(void)
{
	ANO_DT_Send_RCData(	Filter_Rc.THROTTLE,
											Filter_Rc.YAW,
											Filter_Rc.ROLL,
											Filter_Rc.PITCH,
											Filter_Rc.AUX1,
											Filter_Rc.AUX2,
											Filter_Rc.AUX3,
											Filter_Rc.AUX4,
											Battery_Rc,0);
	LED_Show();//LEDָʾ����˸
}

/******************************************************************************
����ԭ�ͣ�	void Task_25HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ25HZ����
*******************************************************************************/ 
void Task_25HZ(void)
{

}

/******************************************************************************
����ԭ�ͣ�	void Task_4HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ4HZ����
*******************************************************************************/ 
void Task_4HZ(void)
{
	Battery_Rc = (uint16_t)(2.0f*ADC_Value[3]/ADC_Value[8]*1.24f*100);//ң�ص�ѹֵ��100��
}
