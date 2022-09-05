   //############################################################
// FILE:  main.c
// Created on: 2017年1月15日
// Author: XQ
// summary: main 主程序的MCU底层初始化，通讯函数
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//DSP/STM32电机控制开发板
//硕历电子
//网址: https://shuolidianzi.taobao.com
//修改日期:2017/1/24
//版本：V17.3-1
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################

#include "stm32f10x.h"
#include "GPIO_int.h"
#include "Timer.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"
#include "Axis_transform.h"  
#include "Svpwm_dq.h"
#include "IQ_math.h"
#include "Tim4_Encoder_PWMDAC.h"
#include "PI_Cale.h"
#include "Task_function.h"
#include "Usart_RS232.h"
#include "CAN.h"
#include "Sensorless_SMO.h"
 
PI_Control   pi_spd = PI_Control_DEFAULTS;
PI_Control   pi_id  = PI_Control_DEFAULTS;
PI_Control   pi_iq  = PI_Control_DEFAULTS;
 
Test         TestPare=Test_DEFAULTS;
TaskTime     TaskTimePare=TaskTime_DEFAULTS;
logic        logicContr=logic_DEFAULTS;
ADCSamp      ADCSampPare=ADCSamp_DEFAULTS;
CLARKE       ClarkeI= CLARKE_DEFAULTS;
PARK         ParkI=PARK_DEFAULTS;
IPARK        IparkU=IPARK_DEFAULTS;
SVPWM        Svpwmdq=SVPWM_DEFAULTS;
IQSin_Cos    AngleSin_Cos=IQSin_Cos_DEFAULTS ;
Angle_SMO    Angle_SMOPare = Angle_SMO_DEFAULTS ;
SMO_Motor    SMO_MotorPare=SMO_Motor_DEFAULTS ;
Speed_est    Speed_estPare=Speed_est_DEFAULTS;
IQAtan       IQAtan_Pare=IQAtan_DEFAULTS;
CANSR      CANSRPare=CANSR_DEFAULTS;
 
int main(void)
{
   Delay(10000);
   SysTickConfig();      // 系统滴答时钟 10ms一次	 
	 logicContr.Run_mode=0;	    // 开环闭环正方转模式
   GPIO_LED485RE_int( ); // RS485 和运行LE
   Init_Gpio_ADC( );     // ADC的引脚初始化
   InitUSART3_Gpio( );   //串口3的IO初始化
   InitCAN_Gpio( );      //CAN通讯的IO初始化
	 Init_Gpio_TIM1_PWM( );// 高级定时器1的6个IO初始化
	 InitThreeHallGpio( ); // 霍尔的IO初始化
	 Init_PWMDAC_Gpio( );  // PWM4的IO作为DAC初始化 
	 Usart3_RS232_init( ); // 串口3的底层初始化
	 CAN_Config( );        // CAN通讯的底层初始化
	 DMA_Configuration( ); //ADC连接DMA读取数据初始化
	 Delay(10000);
   ADC1_Configuration( );    //ADC模式初始化
	 Delay(10000);
   Tim1_PWM_Init( );         //高级定时器1初始化
   Delay(10000);
	 TIM4PWMDAC_Config( );    //TIM4的 作为DAC初始化
	 Delay(10000);
	 Offset_CurrentReading( ); // 电机的母线电流采样偏执电压
	 Delay(10000);
	 PI_Pare_init( );           // 三个双PID参数初始化		
	 SMO_Pare_init( );         // 无感永磁同步参数初始化
	 	 
	 while(1)
	 {
		RunSystimer( );       // 时间任务标志初始化
    CAN_Sendlen( );       //CAN定时发送电机参数
	 	Uart3_RS232TX_sen( ); //串口3通讯的定时发送
	 	ReceiveData_chuli( ); //串口中断接收数据处 理
	  CAN_Receivechuli( );  //CAN通讯中断接收数据处理	 
		CLEAR_flag( );        // 清除时间任务标志 
	 }
}
//===========================================================================
// No more.
//===========================================================================













