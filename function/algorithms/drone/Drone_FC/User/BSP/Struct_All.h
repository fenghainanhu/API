#ifndef _STRUCT_ALL_H_
#define _STRUCT_ALL_H_
#include "stm32f10x.h"

#include "Led.h"
#include "USART3.h"
#include "US_100.h"
#include "Uart.h" 
#include "Motor.h"
#include "math.h"
#include "Timer3.h"
#include "DMA_ADC.h"
#include "Protocol.h"
#include "I2C_MPU6050.h"
#include "SPI_NRF24L01.h"

/******************************************************************************
							�궨��
*******************************************************************************/
#define Lock_Mode (1<<0)//��β
#define Led_Mode  (1<<1)//ҹ��ģʽ
#define PI 3.141592653f

extern uint8_t high_mode;//����ģʽ:0ʧ�ܣ�1ʹ��
extern uint8_t Rc_Lock,start;//1������0����
#define FLY_type 1	
// 3.7VС���� FLY_type == 1;
// 7.4V������ FLY_type == 2;
// ��ˢ������ FLY_type == 3;
//(���棺��ˢ�������ĳ�����Բ�����������������ķɻ������У�����)

#if FLY_type == 1	//3.7VС����

	#define MAX_THROTTLE 999  //�������ֵ
	#define MIN_THROTTLE 0    //������Сֵ
	#define Battery_NUM  1.0f //�������1S
	#define PWM_MAX      999  //PWM��������
	#define	PWM_HZ       3000 //PWMƵ��
	#define BASIC				 1000
	
#elif FLY_type == 2 //7.4V������

	#define MAX_THROTTLE 999  //�������ֵ
	#define MIN_THROTTLE 0    //������Сֵ
	#define Battery_NUM  2.0f //�������2S
	#define PWM_MAX      999  //PWM��������	
	#define	PWM_HZ       3000 //PWMƵ��
	#define BASIC				 1000
	
#elif FLY_type == 3 //��ˢ������

	#define MAX_THROTTLE 1999 //�������ֵ
	#define MIN_THROTTLE 1000 //������Сֵ
	#define Battery_NUM  2.0f //�������3S
	#define PWM_MAX      1999 //PWM��������	
	#define	PWM_HZ       500	//PWMƵ�ʡ�
	#define BASIC				 0
	
#endif


/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Nvic_Init(void);

void EEPROM_INIT(void);
void EEPROM_SAVE_ACC_OFFSET(void);
void EEPROM_READ_ACC_OFFSET(void);
void EEPROM_SAVE_GYRO_OFFSET(void);
void EEPROM_READ_GYRO_OFFSET(void);
void EEPROM_SAVE_PID(void);
void EEPROM_READ_PID(void);
void PID_Reset(void);
/******************************************************************************
							�ṹ������
*******************************************************************************/ 
extern uint16_t offset_high;;//�߶���Ϣ
extern int16_t THROTTLE;
extern float high,Filter_high,old_high,d_high;
extern uint16_t RUN_fre,Debug;//��������Ƶ��
extern float PID[19][3];

/* MPU6050--���ٶȼƽṹ�� */
struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
extern struct _acc offset_acc;

/* MPU6050--�����ǽṹ�� */
struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
extern struct _gyro offset_gyro;
extern struct _out_angle control_angle;
extern struct _out_angle control_gyro;

/* float�ṹ�� */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* ��̬����--�Ƕ�ֵ */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

/* ң������ */
struct _Rc
{
	uint16_t THROTTLE;
	uint16_t YAW;
	uint16_t PITCH;
	uint16_t ROLL;
	
	uint16_t AUX1;
	uint16_t AUX2;
	uint16_t AUX3;
	uint16_t AUX4;
	uint16_t AUX5;
	uint16_t AUX6;
};
extern struct _Rc Rc;

/* pid���� */
struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	
	float output;
};
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;
extern struct _pid control_high;
extern float target_high;

#endif

