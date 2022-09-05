//############################################################
// FILE: Tim1_ISR_MCLoop.c
// Created on: 2017年1月15日
// Author: XQ
// summary: Tim1_ISR_MCLoop
// 定时器1电机控制， 中断环路闭环控制  
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
#include "ThreeHall.h"
#include "Task_function.h"
#include "PI_Cale.h"
#include "Timer.h"
#include "Usart_RS232.h"
#include "Tim4_Encoder_PWMDAC.h"
 
extern  PI_Control   pi_spd ;
extern   ADCSamp    ADCSampPare;
extern   uint16_t   PWM_DUTY[3] ;
extern   Hall       Hall_Three;
extern   logic      logicContr;
extern   TaskTime   TaskTimePare;  
extern   Test       TestPare;

extern  uint16_t  DUTY;

uint16_t  FilK1=328;
uint16_t  FilK2=696;
 
void TIM1_UP_IRQHandler(void)// 触发ADC中断采样和电机环路控制
{
	//此程序更新中断一直执行
	 TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
 
   ThreeHall_huanxkz( ); // 根据三霍尔切换PWM驱动MOS
	 
	 TaskTimePare.PWMZD_count++;
	
   if(TaskTimePare.PWMZD_count==25 ) //2MS时间任务
	 {
	   TaskTimePare.PWMZD_count=0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    	 
		  knob_control( );  //转把电位器调速的控制

      //通讯测试参数输出 
      // 测试参数输出
      TestPare.fact_BUS_Voil= ADCSampPare.BUS_Voltage;
	    TestPare.fact_BUS_Curr= ADCSampPare.BUS_Curr;
	    TestPare.Speed_fact= ( pi_spd.Fbk>>3);
	    TestPare.Speed_target=( pi_spd.Ref>>3);	 	   
   
		  pi_spd.Fbk = Hall_Three.Speed_RPMF ;  //   0---4096RPM	   	 
	    PI_Controller((p_PI_Control)&pi_spd);  // 速度环PI控制 
	    pi_spd.OutF= _IQ10mpy(FilK1,pi_spd.OutF)+_IQ10mpy(FilK2,pi_spd.Out); 
 
		  if(logicContr.Control_Mode==1)
	    {
         DUTY = 2*pi_spd.Ref;  // 闭环  PWM输出
	    } 
	    else if(logicContr.Control_Mode==2)
	    {
         DUTY = pi_spd.OutF;  // 闭环 PWM输出
	    }  
 	 
   }
	 
   ADC_Sample( ); // 电流母线电流电压采集
 	 
   TIM_SetCompare3(TIM4 , Hall_Three.Speed_RPMF); // DAC通道1    
	 TIM_SetCompare4(TIM4 , DUTY);                  // DAC通道2 
} 


//===========================================================================
// No more.
//===========================================================================
