#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "struct_all.h"

uint8_t Bsp_Int_Ok = 0;
uint16_t RUN_fre,Debug;
/******************************************************************************
����ԭ�ͣ�	void Nvic_Init(void)
��    �ܣ�	NVIC��ʼ��
*******************************************************************************/ 
void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
	//Timer3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//Nrf2401�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//����1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//����3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
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
	LED_Init();//LED��ʼ��
	LED_ON_OFF();//LED��˸
	
	Uart1_Init(115200);	//����1��ʼ����δʹ�õ����������ģ�飩
	Uart3_Init(9600); //����3��ʼ��(������ģ��ʹ��)
	Timer3_Init(1000);	//Timer3�ж�1KHZ
	Nvic_Init();		//�ж����ȼ���ʼ��	
	
	PrintString("\r\n RagingFire_Fly V3.5 \r\n");	//�汾��
	Motor_Init();//PWM��ʼ��
	ADC1_Init();//ADC��DMA��ʼ��

	SPI1_Init();//SPI1��ʼ��
	NRF24L01_Init(35,RX);//2401ѡ��40ͨ��������ģʽ
 	NRF24L01_Check(); //���2401�Ƿ�����

	I2C2_Int();//I2C��ʼ��
	while( InitMPU6050()!=1 );//��MPU6050��ʼ�����ɹ����������������
	Calculate_FilteringCoefficient(0.001f,10.f);//����IIR�˲�������

	FLASH_Unlock();//Flash����
	EEPROM_INIT();//��ʼ��ģ���EEPROM����ȡ����
	
	PID_Reset();
		
	Bsp_Int_Ok = 1;
}

/******************************************************************************
����ԭ�ͣ�	void Task_1000HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ1000HZ����
*******************************************************************************/ 
void Task_1000HZ(void)
{
	if( MPU6050_SequenceRead()==0 )//��������ȡ6050���ݼĴ���ʧ��
	{
		I2C_Erro++;//ͳ��I2C�������
		MPU6050_SingleRead();//�ִζ�MPU6050���ݼĴ���
	}
	MPU6050_Compose();//6050���ݺϳ�
	ACC_IIR_Filter(&acc,&filter_acc);//��acc��IIR�˲�
	Gyro_Filter(&gyro,&filter_gyro);//��gyro�������˲�
	Get_Radian(&filter_gyro,&SI_gyro);//���ٶ�����תΪ����
	IMUupdate(SI_gyro.x,SI_gyro.y,SI_gyro.z,filter_acc.x,filter_acc.y,filter_acc.z);//��̬����
	Nrf_Connect();//NRF24L01���Ӻ���
}

/******************************************************************************
����ԭ�ͣ�	void Task_500HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ500HZ����
*******************************************************************************/ 
void Task_500HZ(void)
{
	Control_Gyro(&SI_gyro,&Rc,Rc_Lock);//�ڻ�����
}

/******************************************************************************
����ԭ�ͣ�	void Task_250HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ250HZ����
*******************************************************************************/ 
void Task_250HZ(void)
{
	Get_Eulerian_Angle(&out_angle);//��Ԫ��תŷ����
	Control_Angle(&out_angle,&Rc);//�⻷����
}
/******************************************************************************
����ԭ�ͣ�	void Task_50HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ50HZ����
*******************************************************************************/ 
void Task_50HZ(void)
{
	if(high_mode)	get_high();
}

/******************************************************************************
����ԭ�ͣ�	void Task_1HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ1HZ����
*******************************************************************************/ 
void Task_1HZ(void)
{
	get_Battery_Fly();//���㺽ģ��ص�ѹ
	
	RUN_fre = Debug;
	Debug = 0;
}

void Task_check(void)
{
		Count_1ms++;
		Count_2ms++;
		Count_4ms++;
		Count_20ms++;
		Count_1s++;
		if(Task_check_flag == 1)
		{
			Task_err_flag ++;//ÿ�ۼ�һ�Σ�֤��������Ԥ��������û�����ꡣ
		}
		else
		{
			Task_check_flag = 1;//�ñ�־λ��ѭ�����������
		}
}
void Task(void)//�����������Ϊ1ms���ܵĴ���ִ��ʱ����ҪС��1ms��
{
	if(Task_check_flag == 1)
	{
		if(Count_1ms>=1)
		{	
			Count_1ms = 0;
			Task_1000HZ();
		}
		if(Count_2ms>=2)
		{
			Count_2ms = 0;
			Task_500HZ();
		}
		if(Count_4ms>=4)
		{
			Count_4ms = 0;
			Task_250HZ();
		}
		if(Count_20ms>=20)
		{
			Count_20ms = 0;
			Task_50HZ();
		}
		if(Count_1s>=1000)
		{
			Count_1s = 0;
			Task_1HZ();
		}
		Task_check_flag = 0;
	}
}
			
