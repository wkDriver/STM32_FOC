//############################################################
// FILE: stm32f10x_it.c
// Created on: 2017年1月15日
// Author: XQ
// summary: 部分中断控制
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

#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "GPIO_int.h"
#include "Timer.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"

extern  TaskTime       TaskTimePare;   
 
void SysTick_Handler(void)
{
 TaskTimePare.IntClock_10ms=1;
}
 void TIM1_BRK_IRQHandler(void)
{
	Stop_Motor( );
  TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
   
}
 

//===========================================================================
// No more.
//===========================================================================
