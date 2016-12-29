#include "struct_all.h"

uint8_t USART3_TxCount=0;
uint8_t USART3_Count=0;
static uint8_t USART3_TxBuff[256];	//���ڷ��ͻ�����

/******************************************************************************
����ԭ�ͣ�	void Uart1_Init(uint32_t baud)
��    �ܣ�	���ڳ�ʼ��
��    ����  baud��������
*******************************************************************************/ 
void Uart3_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure
	;
	/* ��GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* ��USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ��USART Rx��GPIO����Ϊ��������ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����USART���� */
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һλֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//����+����ģʽ
	USART_Init(USART3, &USART_InitStructure);
	
	/* ʹ�ܴ��ڽ����ж� */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART3, ENABLE);
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);//�巢����ɱ�־λ
}

/******************************************************************************
����ԭ�ͣ�	void USART3_IRQHandler(void)
��    �ܣ�	����3�ж�
*******************************************************************************/ 
void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)//ORE�ж�
	{
		USART_ReceiveData(USART3);
	}
	
	if(USART3->SR & USART_SR_TC && (USART3->CR1 & USART_CR1_TXEIE) )//�����ж�
	{
		USART3->DR = USART3_TxBuff[USART3_TxCount++];//дDR����жϱ�־          
		if(USART3_TxCount == USART3_Count)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;//�ر�TXE�ж�
		}
	}
	
	if(USART3->SR & USART_SR_RXNE)//�����ж�
	{
		uint8_t com_data = USART3->DR;
		receive_higt(com_data);
	}
}

//��HEX����ʽ���U8������
void USART3_PrintHexU8(uint8_t data)
{
	USART3_TxBuff[USART3_Count++] = data;  
	if(!(USART3->CR1 & USART_CR1_TXEIE))
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //��TXE�ж�
}
