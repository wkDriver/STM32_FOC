//############################################################
// FILE: Tim1_ISR_MCLoop.c
// Created on: 2017年1月15日
// Author: XQ
// summary: Tim1_ISR_MCLoop
//  定时器1电机控制， 中断环路闭环控制  
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
#include "Tim1_ISR_MCLoop.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"
#include "GPIO_int.h"
#include "Axis_transform.h"
#include "Svpwm_dq.h"
#include "VF_angle.h"
#include "Task_function.h"
#include "Timer.h"
#include "Usart_RS232.h"
#include "Tim4_Encoder_PWMDAC.h"
 
extern   ADCSamp    ADCSampPare;
extern   uint16_t   PWM_DUTY[3];
extern   logic      logicContr;
extern   TaskTime   TaskTimePare;  
extern   CLARKE     ClarkeI;
extern   PARK       ParkI;
extern   IPARK      IparkU;
extern   SVPWM      Svpwmdq;
extern   VF_Angle   VF_AnglePare;
extern   IQSin_Cos  AngleSin_Cos;
extern   Test       TestPare;
 
 
uint16_t  FilK1=328;
uint16_t  FilK2=696;

void TIM1_UP_IRQHandler(void)// 触发ADC中断采样和电机环路控制
{
	//此程序更新中断一直执行
	 TIM_ClearFlag(TIM1, TIM_FLAG_Update); //清除标志位

	 VFAngle_cale( ); // VF角度函数

	 TaskTimePare.PWMZD_count++;
   if(TaskTimePare.PWMZD_count==25 ) // 2ms的事件任务
	 {
	   TaskTimePare.PWMZD_count=0;
     VF_start_control( );   // VF曲线控制函数                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
     knob_control( );       //转把电位器调速的控制

// 测试参数输出		 
     TestPare.fact_BUS_Voil= ADCSampPare.BUS_Voltage;
	   TestPare.fact_BUS_Curr= ADCSampPare.BUS_Curr;
	   TestPare.Speed_fact= 1 ;       
	   TestPare.Speed_target= 1;	
   }
	
	  ADC_Sample( );   // 相电流母线电流电压采集
  
  	ClarkeI.As=ADCSampPare.PhaseU_Curr;
	  ClarkeI.Bs=ADCSampPare.PhaseV_Curr;

	  CLARKE_Cale((p_CLARKE)&ClarkeI );  // CLARKE变换

	  ParkI.Alpha=ClarkeI.Alpha;
	  ParkI.Beta=ClarkeI.Beta;
 	 	
	  AngleSin_Cos.IQAngle=VF_AnglePare.Angle_theta;
 
	  IQSin_Cos_Cale((p_IQSin_Cos)&AngleSin_Cos); //磁极位置角度，正余弦计算函数

		ParkI.Sine =AngleSin_Cos.IQSin;  
	  ParkI.Cosine =AngleSin_Cos.IQCos;   

	  PARK_Cale((p_PARK)&ParkI); // park变换
 
	  IparkU.Ds=0;   // 采用速度 
	  IparkU.Qs= VF_AnglePare.VF_Vq  ; //VF的电压矢量值
      
    IparkU.Sine=AngleSin_Cos.IQSin;
    IparkU.Cosine=AngleSin_Cos.IQCos;
	  IPARK_Cale((p_IPARK)&IparkU);   //反park变换
 	
	  Svpwmdq.Ualpha =IparkU.Alpha;
	  Svpwmdq.Ubeta = IparkU.Beta;

    SVPWM_Cale((p_SVPWM)&Svpwmdq); //SVPWM计算占空比

    Svpwm_Outpwm( );  // PWM输出  
		
    TIM_SetCompare3(TIM4 , (VF_AnglePare.Angle_theta>>5)); // DAC通道1 硬件电路RC滤波输出直流分量
	  TIM_SetCompare4(TIM4 , ( IparkU.Qs )>>5); // DAC通道2 硬件电路RC滤波输出直流分量
} 


//===========================================================================
// No more.
//===========================================================================
