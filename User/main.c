#include "stm32f10x.h"   // Device header
#include "delay.h"
#include "oled.h"
#include "Key.h"
#include "Servo.h"
#include "PWM.h"
#include "MPU6050.h"
#include "swich.h"
#include "bluetooth.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define Kp 100.0f                        // ��������֧�������������ٶȼ�/��ǿ��
#define Ki 0.002f                // ��������֧���ʵ�������ƫ�����ν�
#define halfT 0.001f                // �������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;          // ��Ԫ����Ԫ�أ�������Ʒ���
float exInt = 0, eyInt = 0, ezInt = 0;        // ��������С�������
 
float Yaw,Pitch,Roll;  //ƫ���ǣ������ǣ�������

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
        float norm;
        float vx, vy, vz;
        float ex, ey, ez;  
 
        // ����������
        norm = sqrt(ax*ax + ay*ay + az*az);      
        ax = ax / norm;                   //��λ��
        ay = ay / norm;
        az = az / norm;      
 
        // ���Ʒ��������
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
 
        // ���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�
        ex = (ay*vz - az*vy);
        ey = (az*vx - ax*vz);
        ez = (ax*vy - ay*vx);
 
        // ������������������
        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;
 
        // ������������ǲ���
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
 
        // ������Ԫ���ʺ�������
        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
 
        // ��������Ԫ
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 = q0 / norm;
        q1 = q1 / norm;
        q2 = q2 / norm;
        q3 = q3 / norm;
 
        Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,ת��Ϊ����
        //Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv
        //Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;                //�˴�û�м�ֵ��ע��
}



float changeA(int16_t x)
{
	int xx = x;
	
	return (float)(xx*16*9.8/32768);
}
float changeG(int16_t x)
{
	int xx = x;
	
	return (float)(xx*2000/32768);
}
uint8_t KeyNum;//�жϰ����Ƿ��£�����KeyNum��Ϊ1
float Angle = 0;


int main(void)
{
	Swich_Init();
	Delay_init();
	OLED_Init();
	Servo_Init();
	Key_Init();
	OLED_Clear();
	MPU6050_Init();
	
	OLED_Clear();
	
	OLED_ShowCHinese(29, 0 ,0);
	OLED_ShowCHinese(47, 0 ,1);
	OLED_ShowCHinese(65, 0 ,2);
	OLED_ShowCHinese(83, 0 ,3);	

	OLED_ShowCHinese(38, 2, 4);
	OLED_ShowCHinese(56, 2, 5);
	OLED_ShowCHinese(74, 2, 6);
	
	OLED_ShowCHinese(38, 4, 7);	
	OLED_ShowCHinese(56, 4, 8);
	OLED_ShowCHinese(74, 4, 9);
	
	OLED_ShowCHinese(38, 6, 10);
	OLED_ShowCHinese(56, 6, 11);	
	OLED_ShowCHinese(74, 6, 12);	
	
	RCC_Configuration();//USART1ʱ��ʹ�ܣ�GPIOʱ��ʹ��
	NVIC_Configuration2();//�жϽ���ʹ��
	GPIO_Config_Init();	//USART1�ܽ�PA9,PA10��ʼ��
	USART_Config_Init();//USART1���ã�������9600��8λ���ݣ�1λֹͣλ����У��
	
	Servo_SetAngle(65);

	while(1)
	{	
		KeyNum = Key_GetNum();
		Delay_ms(100);
		if(KeyNum == 1)
		{
			//GPIO4������ߵ�ƽ������ʼ����
			GPIO_SetBits(GPIOB, GPIO_Pin_4);
				
			int16_t Ax, Ay, Az, Gx, Gy, Gz;
			
			float a;
			while(1)
			{		
				
				MPU6050_GetData(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
				IMUupdate(changeG(Gx),changeG(Gy),changeG(Gz),changeA(Ax),changeA(Ay),changeA(Az));
				
				Pitch -= 10;
				a = 2.5 * Pitch-0.005*changeG(Gy);
				a = (a > 20) ? 12:a;
				a = (a < -20) ? -12:a;
				Servo_SetAngle(65+a);
				
				int i; 
				uint16_t UartRxLen=25;
				char UartRxbuf[21]={"���ٶ�:"};
				char StrAy[10];
				sprintf(StrAy, "%.2f",changeA(Ay));
				strcat(UartRxbuf, StrAy);
				strcat(UartRxbuf, "\n");
				strcat(UartRxbuf, "��б��:");
				char StrTurn[10];
				sprintf(StrTurn, "%.2f",Pitch);
				strcat(UartRxbuf, StrTurn);

				uint8_t UartIntRxbuf[512];//���ջ���
				for(i=0;i<UartRxLen;i++)
				{
					USART_SendData(USART1, UartRxbuf[i]);//��������
					while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC));//��������ж�
				}
				Delay_ms(120);//�ȴ�100ms
				//�ٰ�һ�¾��˳�
				KeyNum = Key_GetNum();
				if(KeyNum == 1)
				{
					break;
				}
			}
			
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		}
	}
}
