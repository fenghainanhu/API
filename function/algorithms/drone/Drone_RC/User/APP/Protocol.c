#include "struct_all.h"

/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
#define u8 uint8_t

dt_flag_t f;					//��Ҫ�������ݵı�־
u8 data_to_send[50];	//�������ݻ���
float PID[18][3];

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
//��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
	NRF_Send_TX(dataToSend,32);
}

static void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
	u8 i;
	u8 sum = 0;
	
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;

	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	ANO_DT_Send_Data(data_to_send, 7);
}

/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
void ANO_DT_Data_Receive_Prepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Send_Data(RxBuffer,32);//�յ�����ֱ��ת�����ɿ�
	}
	else
		state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
u16 flash_save_en_cnt = 0;

void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	u8 i;
	
	for(i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
		{
//			Do_ACC_Offset(); //У�����ٶȼ�
		}
		else if(*(data_buf+4)==0X02)
		{
//			Do_GYRO_Offset();//У��������
		}
		else if(*(data_buf+4)==0X03)
		{
//			Do_ACC_Offset(); //У�����ٶȼ�	
//			Do_GYRO_Offset();//У��������		
		}
		else if(*(data_buf+4)==0X04)
		{
			//Mag_CALIBRATED = 1;
		}
		else if((*(data_buf+4)>=0X021)&&(*(data_buf+4)<=0X26))
		{
			//acc_3d_calibrate_f = 1;
		}
		else if(*(data_buf+4)==0X20)
		{
			//acc_3d_step = 0; //�˳���6��У׼����0
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			f.send_pid1 = 1;
			f.send_pid2 = 1;
			f.send_pid3 = 1;
			f.send_pid4 = 1;
			f.send_pid5 = 1;
			f.send_pid6 = 1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		//��ȡ�汾��Ϣ
		{
			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//�ָ�Ĭ�ϲ���
		{
			
		}
	}

	if(*(data_buf+2)==0X03)
	{
		Rc.THROTTLE = (vs16)(*(data_buf+4)<<8)|*(data_buf+5) ;
		Rc.YAW 			= (vs16)(*(data_buf+6)<<8)|*(data_buf+7) ;
		Rc.ROLL 		= (vs16)(*(data_buf+8)<<8)|*(data_buf+9) ;
		Rc.PITCH		= (vs16)(*(data_buf+10)<<8)|*(data_buf+11) ;
		Rc.AUX1 		= (vs16)(*(data_buf+12)<<8)|*(data_buf+13) ;
		Rc.AUX2			= (vs16)(*(data_buf+14)<<8)|*(data_buf+15) ;
		Rc.AUX3 		= (vs16)(*(data_buf+16)<<8)|*(data_buf+17) ;
		Rc.AUX4 		= (vs16)(*(data_buf+18)<<8)|*(data_buf+19) ;
	}

	if(*(data_buf+2)==0X10)								//PID1
    {
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID[0][1]  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID[0][2]  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        PID[1][0] = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID[1][1] = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID[1][2] = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        PID[2][0] 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        PID[2][1] 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        PID[2][2] 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
			
				flash_save_en_cnt = 1;
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        PID[0][0] = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID[0][0] = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID[0][0] = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);

				flash_save_en_cnt = 1;
    }
    if(*(data_buf+2)==0X12)								//PID3
    {	
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        PID[0][0]  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
			
        PID[0][0] = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        PID[0][0] = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        PID[0][0] = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
			
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        PID[0][0] 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);

				flash_save_en_cnt = 1;
    }
		if(*(data_buf+2)==0X13)								//PID4
		{
				PID[0][0]  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
				PID[0][0]  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
				PID[0][0]  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );

				PID[0][0] = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
				PID[0][0] = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
				PID[0][0] = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );

				PID[0][0] 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
				PID[0][0] 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
				PID[0][0] 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
				ANO_DT_Send_Check(*(data_buf+2),sum);

				flash_save_en_cnt = 1;
		}
		if(*(data_buf+2)==0X14)								//PID5
		{
			ANO_DT_Send_Check(*(data_buf+2),sum);
		}
		if(*(data_buf+2)==0X15)								//PID6
		{
			ANO_DT_Send_Check(*(data_buf+2),sum);
		}
}

void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=hardware_type;
	data_to_send[_cnt++]=BYTE1(hardware_ver);
	data_to_send[_cnt++]=BYTE0(hardware_ver);
	data_to_send[_cnt++]=BYTE1(software_ver);
	data_to_send[_cnt++]=BYTE0(software_ver);
	data_to_send[_cnt++]=BYTE1(protocol_ver);
	data_to_send[_cnt++]=BYTE0(protocol_ver);
	data_to_send[_cnt++]=BYTE1(bootloader_ver);
	data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Speed(float x_s,float y_s,float z_s)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	vs16 _temp;
	
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x0B;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(0.1f *x_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0.1f *y_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0.1f *z_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);

}

void ANO_DT_Send_Location(u8 state,u8 sat_num,s32 lon,s32 lat,float back_home_angle)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	vs16 _temp;
	vs32 _temp2;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x04;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=state;
	data_to_send[_cnt++]=sat_num;
	
	_temp2 = lon;//����
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);	
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	_temp2 = lat;//γ��
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);	
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	
	_temp = (s16)(100 *back_home_angle);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);

}


void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	vs16 _temp;
	vs32 _temp2 = alt;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
/////////////////////////////////////////
	_temp = 0;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_Senser2(s32 bar_alt,u16 csb_alt)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE3(bar_alt);
	data_to_send[_cnt++]=BYTE2(bar_alt);
	data_to_send[_cnt++]=BYTE1(bar_alt);
	data_to_send[_cnt++]=BYTE0(bar_alt);

	data_to_send[_cnt++]=BYTE1(csb_alt);
	data_to_send[_cnt++]=BYTE0(csb_alt);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	
	aux4 = 0;
	if(Mode)//Mode����
		aux4 |= 	Lock_Mode;//��βģʽ
	if(Fun)//Fun����
		aux4 |=  Led_Mode;//ҹ��ģʽ
		
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, 32);
}
void ANO_DT_Send_RCData_To_PC(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	
	aux4 = 0;
	if(Mode)//Mode����
		aux4 |= 	Lock_Mode;//��βģʽ
	if(Fun)//Fun����
		aux4 |=  Led_Mode;//ҹ��ģʽ
		
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart_Send(data_to_send, _cnt);
}
void ANO_DT_Send_Power_To_PC(u16 votage, u16 current)
{
	u8 _cnt=0,i;
	u8 sum = 0;
	u16 temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = current;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Uart_Send(data_to_send, _cnt);
}


//	data_to_send[0]=0xAA;	����֡ͷ����Զ�ǹ̶���һ����
//  data_to_send[1];			����1
//  data_to_send[2];			����2
//  data_to_send[3];			����3
//  data_to_send[4];			����4
//  data_to_send[5];			��
//  data_to_send[6];			��
//  data_to_send[7];			��
//  data_to_send[8];			��
//  data_to_send[9];			Enter
//  data_to_send[10];			Esc
//  data_to_send[11];			Backspace
//  data_to_send[12];			A
//  data_to_send[13];			B
//  data_to_send[14];			���
//  data_to_send[15];			��У��
