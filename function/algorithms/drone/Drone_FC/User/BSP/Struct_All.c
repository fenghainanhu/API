#include "EEPROM.h"
#include "Filter.h"
#include "struct_all.h"

struct _acc  acc;			//ԭʼ����
struct _gyro gyro;
////////////////////////////////////////////
struct _acc  filter_acc;	//�˲�������
struct _gyro filter_gyro;
////////////////////////////////////////////
struct _acc  offset_acc;	//��ƫ����
struct _gyro offset_gyro;
////////////////////////////////////////////
struct _SI_float  SI_acc;	//���ٶ����ݣ�m/s2��
struct _SI_float  SI_gyro;	//���ٶ����ݣ�rad��
struct _SI_float  roll_3D_gyro; //�������ٶ�����
////////////////////////////////////////////
struct _Rc Rc;				//ң��ͨ��
struct _out_angle out_angle;//��̬����-�Ƕ�ֵ

/* pid */
struct _pid pitch;
struct _pid roll;
struct _pid gyro_pitch;
struct _pid gyro_roll;
struct _pid gyro_yaw;
struct _pid control_high;


/******************************************************************************
����ԭ�ͣ�	void PID_Reset(void)
��    �ܣ�	����PID����
*******************************************************************************/ 
void PID_Reset(void)
{
//�������ݳ�ʼ��
	Rc.PITCH 		= 1500;
	Rc.ROLL 		= 1500;
	Rc.YAW 			= 1500;
	Rc.THROTTLE = 1500;

	Rc.AUX1 	= 1500;
	Rc.AUX2		= 1500;
	Rc.AUX3		= 1500;
//����PID������ʼ��
	
	//�ǶȻ�
	PID[1][0] = roll.kp = pitch.kp = 2.0f;
	PID[1][1] = roll.ki = pitch.ki = 0.04f;
	PID[1][2] = roll.kd = pitch.kd = 0.0f;
	//���ٶȻ�
	PID[2][0] = gyro_roll.kp = gyro_pitch.kp = 2.0f;
	PID[2][1] = gyro_roll.ki = gyro_pitch.ki = 0.0f;
	PID[2][2] = gyro_roll.kd = gyro_pitch.kd = 0.0f;
	
	//ƫ����
	PID[3][0] = gyro_yaw.kp = 8.0f;
	PID[3][1] = gyro_yaw.ki = 0.1f;
	PID[3][2] = gyro_yaw.kd = 0.0f;
//����������PID��ʼ��
	if(FLY_type==1)//С���ᶨ�߲���
	{
		PID[4][0] = control_high.kp = 1.0f;
		PID[4][1] = control_high.ki = 0.1f;
		PID[4][2] = control_high.kd = 30.0f;
	}
	if(FLY_type==2)//�����ᶨ�߲���
	{
		PID[4][0] = control_high.kp = 0.4f;
		PID[4][1] = control_high.ki = 0.1f;
		PID[4][2] = control_high.kd = 20.0f;
	}
	EEPROM_SAVE_PID();
}
///////////////////////////////////////////////////////////
#define EEPROM_offset_acc_x		0
#define EEPROM_offset_acc_y		1
#define EEPROM_offset_acc_z		2
#define EEPROM_offset_gyro_x	3
#define EEPROM_offset_gyro_y	4
#define EEPROM_offset_gyro_z	5
#define EEPROM_PID_ROL_P		6
#define EEPROM_PID_ROL_I		7
#define EEPROM_PID_ROL_D		8
#define EEPROM_PID_PIT_P		9
#define EEPROM_PID_PIT_I		10
#define EEPROM_PID_PIT_D		11
#define EEPROM_PID_YAW_P		12
#define EEPROM_PID_YAW_I		13
#define EEPROM_PID_YAW_D		14
#define EEPROM_SUM				15

uint16_t data_x,data_y,data_z;
uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A10,0x9A12,0x9A13,0x9A14,0x9A15
};//�����ַ

void EEPROM_INIT(void)//��ʼ��ģ���EEPROM����ȡ����
{
	EE_Init();	
	EEPROM_READ_ACC_OFFSET();
	EEPROM_READ_GYRO_OFFSET();
	EEPROM_READ_PID();
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_SUM], &data_x);
	if(data_x != EEPROM_SUM)//����Ƿ��״��Է�
	{
		__set_PRIMASK(1);//��ȫ���ж�
		EE_WriteVariable(VirtAddVarTab[EEPROM_SUM], EEPROM_SUM);
		 __set_PRIMASK(0);//���ж�
		PID_Reset();//�״��Էɣ�����PID����ΪĬ��ֵ
	}
}

void EEPROM_SAVE_ACC_OFFSET(void)	//����acc��ƫ
{
	__set_PRIMASK(1);//��ȫ���ж�
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_x], offset_acc.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_y], offset_acc.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_acc_z], offset_acc.z);
	 __set_PRIMASK(0);//���ж�
}
void EEPROM_READ_ACC_OFFSET(void)	//��ȡacc��ƫ
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_acc_z], &data_z);
	offset_acc.x = (int16_t) data_x;
	offset_acc.y = (int16_t) data_y;
	offset_acc.z = (int16_t) data_z;
}
void EEPROM_SAVE_GYRO_OFFSET(void)	//����gyro��ƫ
{
	 __set_PRIMASK(1);//��ȫ���ж�
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_x], offset_gyro.x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_y], offset_gyro.y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_offset_gyro_z], offset_gyro.z);
	 __set_PRIMASK(0);//���ж�
}
void EEPROM_READ_GYRO_OFFSET(void)	//��ȡgyro��ƫ
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_x], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_y], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_offset_gyro_z], &data_z);
	offset_gyro.x = (int16_t) data_x;
	offset_gyro.y = (int16_t) data_y;
	offset_gyro.z = (int16_t) data_z;
}
void EEPROM_SAVE_PID(void)	//����PID����
{
	 __set_PRIMASK(1);//��ȫ���ж�
	data_x = (int16_t) (roll.kp*10);
	data_y = (int16_t) (roll.ki*1000);
	data_z = (int16_t) (roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ROL_D], data_z);

	data_x = (int16_t) (gyro_roll.kp*10);
	data_y = (int16_t) (gyro_roll.ki*1000);
	data_z = (int16_t) (gyro_roll.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PIT_D], data_z);

	data_x = (int16_t) (gyro_yaw.kp*10);
	data_y = (int16_t) (gyro_yaw.ki*1000);
	data_z = (int16_t) (gyro_yaw.kd);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_YAW_D], data_z);
	__set_PRIMASK(0);//���ж�
}
void EEPROM_READ_PID(void)	//��ȡPID����
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ROL_D], &data_z);
	roll.kp = (float) data_x/10.0f;
	roll.ki = (float) data_y/1000.0f;
	roll.kd = (float) data_z;
	pitch.kp = roll.kp;
	pitch.ki = roll.ki;
	pitch.kd = roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PIT_D], &data_z);
	gyro_roll.kp = (float) data_x/10.0f;
	gyro_roll.ki = (float) data_y/1000.0f;
	gyro_roll.kd = (float) data_z;
	gyro_pitch.kp = gyro_roll.kp;
	gyro_pitch.ki = gyro_roll.ki;
	gyro_pitch.kd = gyro_roll.kd;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_YAW_D], &data_z);
	gyro_yaw.kp = (float) data_x/10.0f;
	gyro_yaw.ki = (float) data_y/1000.0f;
	gyro_yaw.kd = (float) data_z;
}
