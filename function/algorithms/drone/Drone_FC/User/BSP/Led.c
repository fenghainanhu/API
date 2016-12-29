#include "struct_all.h"
#define LED TIM4->CCR1
float light = 1.5*PI;//����ʼ
/******************************************************************************
����ԭ�ͣ�	static void Delay_led(u16 n)
��    �ܣ�	��ʱ
*******************************************************************************/ 
void Delay_led(u16 n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8500;j++);
}

/******************************************************************************
����ԭ�ͣ�	void LED_Init(void)
��    �ܣ�	LED��ʼ��
*******************************************************************************/ 
static void Tim4_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//PWMƵ�� = 72000000 / 72 / 2000 = 500hz
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; //PWM��������	 
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //����������ΪTIM4ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��4��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseStructure��ָ���Ĳ�����ʼ������TIM4
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM2����
	
	LED = 0;
}

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�ܺ������õ�ʱ��
	
	GPIO_InitStructure.GPIO_Pin =  LED2_Pin;	//LED2����Դ�Ե�ָʾ��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; 				//���ú�����ʹ�õ��ùܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Tim4_init();
	
	PrintString("\r\n �����Ƴ�ʼ����ɣ�");
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
����ԭ�ͣ�	void LED_ON_OFF(void)
��    �ܣ�	LED������˸
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}	
	LED2_OFF;
	LED3_OFF;
	Delay_led(200);
	
	light = 1.5*PI;
}
void LED_FLASH(void)
{
		LED3_ON;
		Delay_led(100);
		LED3_OFF;
		Delay_led(100);
		LED3_ON;
		Delay_led(100);
		LED3_OFF;
		Delay_led(100);
		light = 1.5*PI;
}

