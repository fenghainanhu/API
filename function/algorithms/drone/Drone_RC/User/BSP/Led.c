#include "struct_all.h"

#include "math.h"
#define LED TIM2->CCR4
#define PI 3.141592653f
float light = 1.5*PI;//����ʼ
/******************************************************************************
����ԭ��:	static void Delay_led(uint16_t n)
��������:	��ʱ
*******************************************************************************/ 
static void Delay_led(uint16_t n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8888;j++);
} 

/******************************************************************************
����ԭ�ͣ�	static void Tim2_init(void)
��    �ܣ�	������PWM��ʼ��
*******************************************************************************/ 
static void Tim2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//ʹ�ܸ��ù���ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
	
	//PWMƵ�� = 72000000 / 72 / 2000 = 500hz
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; //PWM��������	 
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��4��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseStructure��ָ���Ĳ�����ʼ������TIM4
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2����
	
	LED = 0;
}


/******************************************************************************
����ԭ��:	void LED_Init(void)
��������:	LED��ʼ��
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Pin =  Bee_Pin;	//״ָ̬ʾ��
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(Bee_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDRed_Pin;	//������
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(LEDRed_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDGreen_Pin;//ͨ��ָʾ��
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LEDGreen_GPIO, &GPIO_Structure);
	
	Tim2_init();
	
	PrintString("\rLED_Init_succeed...\n");
}

/******************************************************************************
����ԭ��:	void LED_ON_OFF(void)
��������:	LED������˸
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LEDRed_ON;	LEDGreen_OFF;Bee_ON;
		Delay_led(200);
		LEDGreen_ON;	LEDRed_OFF;Bee_OFF;
		Delay_led(200);	
	}	
	LEDRed_OFF;
	LEDGreen_OFF;
	Delay_led(100);
}

/******************************************************************************
����ԭ�ͣ�	void Breath_LED(float speed)
��    ����	speed �����ٶ�
��    �ܣ�	������
*******************************************************************************/ 
void Breath_LED(float speed)
{
	light += speed;
	if(light >=2*PI)	light = 0.0f;
	LED = 1999*( (sin(light)+1) / 2);
}
/******************************************************************************
����ԭ�ͣ�	void LED_Show(void)
��    �ܣ�	LEDָʾ����˸
*******************************************************************************/ 
void LED_Show(void)
{	
	static uint8_t i;
	
	if( Battery_Rc>300 && Battery_Rc<370 )//��ѹ����
	{
		Bee_ON;
		Breath_LED(0.4);
		if(Battery_Rc<350) //��ѹ����3.5ֹͣ����
		{
			i++;
			if(i>=200)
			{
				TIM2->CCR4 = 1500 ;//΢��
				LEDGreen_OFF;
				while(1);
			}
		}
		else i=0;
	}
	else if(Mode)//��βģʽ����������
	{
		Bee_OFF;
		Breath_LED(0.02);//��ɫLED4HZ��˸
	}
	else//����βģʽ�����ٺ������������񾭣�
	{
		Bee_ON;
		Breath_LED(0.2);//��ɫLED1HZ��˸
	}
}
