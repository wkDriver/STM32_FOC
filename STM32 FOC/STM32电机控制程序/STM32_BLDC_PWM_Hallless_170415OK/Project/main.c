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
#include "BEF_Hall.h"
#include "IQ_math.h"
#include "Tim4_Encoder_PWMDAC.h"
#include "VF_angle.h" 
#include "PI_Cale.h"
#include "Task_function.h"
#include "Usart_RS232.h"
#include "CAN.h"
 
PI_Control   pi_spd = PI_Control_DEFAULTS;
 
Test         TestPare=Test_DEFAULTS;
TaskTime     TaskTimePare=TaskTime_DEFAULTS;
logic        logicContr=logic_DEFAULTS;
ADCSamp      ADCSampPare=ADCSamp_DEFAULTS;
Hall         BEF_ThreeCAP=Hall_DEFAULTS;
VF_start     VF_startPare=VF_start_DEFAULTS ;
CANSR        CANSRPare=CANSR_DEFAULTS;

int main(void)
{
   Delay(10000);
   SysTickConfig();      // 系统滴答时钟 10ms一次  
	 logicContr.Run_mode=1; // 正反转控制 
   GPIO_LED485RE_int( ); // RS485 和运行LE
   Init_Gpio_ADC( );     // ADC的引脚初始化
	 InitUSART3_Gpio( );   // 串口3IO初始化
   InitCAN_Gpio( );      // can通讯接口IO初始化
 	 Init_Gpio_TIM1_PWM( );// 高级定时器1的6个IO初始化 
	 InitThree_BEF_Gpio( ); //  反电动势过零比较IO初始化
	 BEF_ThreeCAPPara_init( );//反电动势过零比较参数初始化	
	 VF_start_init( );        //开环启动控制
 	 Init_PWMDAC_Gpio( );  // PWM4的IO作为DAC初始化
	 Usart3_RS232_init( );  // 串口通讯的初始化
	 CAN_Config( );          //CAN通讯初始化
 	 DMA_Configuration( ); //ADC连接DMA读取数据初始化
	 Delay(10000);
   ADC1_Configuration( );  //ADC模式初始化 
   Delay(10000);
   Tim1_PWM_Init( );       //高级定时器1初始化
   Delay(10000);     
	 TIM4PWMDAC_Config( );   //TIM4的 作为DAC初始化
	 Delay(10000);
	 Offset_CurrentReading( ); // 电机的母线电流采样偏执电压
	 Delay(10000);	
	 PI_Pare_init( );          // 三个双PID参数初始化		
   
	while(1)
	 {
		RunSystimer( );       // 时间任务标志初始化
    CAN_Sendlen( );       //CAN定时发送电机参数
	 	Uart3_RS232TX_sen( ); //串口3通讯的定时发送
	 	ReceiveData_chuli( ); //串口中断接收数据处理
	  CAN_Receivechuli( );  //CAN通讯中断接收数据处理	 
		CLEAR_flag( );        // 清除时间任务标志 
	 }
}
 
//===========================================================================
// No more.
//===========================================================================

