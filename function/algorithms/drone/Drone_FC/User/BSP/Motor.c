#include "struct_all.h"

/******************************************************************************
����ԭ�ͣ�	static void Tim2_init(void)
��    �ܣ�	tim2��ʼ��
*******************************************************************************/ 
static void Tim2_init(void)
{
	uint16_t TIM_Prescaler;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_Prescaler = 72000000/(PWM_MAX+1)/PWM_HZ -1;
	TIM_TimeBaseStructure.TIM_Period = PWM_MAX; //PWM��������	
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler; //����������ΪTIM2ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��4��Ƶ

	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseStructure��ָ���Ĳ�����ʼ������TIM2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIM2��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2����
}

/******************************************************************************
����ԭ�ͣ�	void Motor_Init(void)
��    �ܣ�	PWM��ʼ��
*******************************************************************************/ 
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�ܵ���õ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
	//���õ��ʹ�õ��ùܽ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Tim2_init();
	PrintString("\r\n PWM      ��ʼ����ɣ�");
}

/******************************************************************************
����ԭ�ͣ�	void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4)
��    �ܣ�	�������
*******************************************************************************/ 
void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4)
{	
	TIM2->CCR1 = duty1;
	TIM2->CCR2 = duty2;
	TIM2->CCR3 = duty3;
	TIM2->CCR4 = duty4;
}
