//############################################################
// FILE: Timer.c
// Created on: 2017年1月11日
// Author: XQ    
// summary: Timer    
// 定时器1电机控制，
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
#include "Timer.h"
#include "GPIO_int.h"
#include "Tim1_PWM.h"

extern  TaskTime       TaskTimePare;    
 
void SysTickConfig(void)
{
  /* Setup SysTick Timer for 1ms interrupts  */
  if (SysTick_Config(SystemCoreClock /100))   //  10ms
  {
    /* Capture error */ 	
    while (1);
  }
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}
 
void RunSystimer(void)
{
   if(TaskTimePare.IntClock_10ms==1)
    {          
     TaskTimePare.IntClock_10ms=0;
	   TaskTimePare.Tim10ms_flag = 1;
	 if(++TaskTimePare.Tim100ms_count >=10)
	   {
	   TaskTimePare.Tim100ms_count=0;
	   TaskTimePare.Tim100ms_flag=1;
	   }
    }
   if(TaskTimePare.Tim100ms_flag==1)
	   {		   		 
	    if(++TaskTimePare.Tim500ms_count >=5)
	    {	      
	       TaskTimePare.Tim500ms_count=0;
	       TaskTimePare.Tim500ms_flag=1;
      }
	   }
    if(TaskTimePare.Tim500ms_flag==1)
	   {            
	    if(++TaskTimePare.Tim1s_count >=2)
	    {	               
	       TaskTimePare.Tim1s_count=0;
	       TaskTimePare.Tim1s_flag=1;
      }
	   }
      if(TaskTimePare.Tim1s_flag == 1)
	   {      
         LED1_Toggle( );   
	   if(++TaskTimePare.Tim10s_count >=10)
	    {
		TaskTimePare.Tim10s_count = 0;
		TaskTimePare.Tim10s_flag = 1;
	    }
	   } 
    if(TaskTimePare.Tim10s_flag == 1)
	   {                   
	   if(++TaskTimePare.Tim1min_count >=6)
	    {
	  	TaskTimePare.Tim1min_count = 0;
		  TaskTimePare.Tim1min_flag = 1;
	    }
	   }
}

void CLEAR_flag(void)   //清除事件标志位
{
	TaskTimePare.Tim10ms_flag=0;
	TaskTimePare.Tim100ms_flag=0;
	TaskTimePare.Tim500ms_flag=0;
	TaskTimePare.Tim1s_flag=0;
	TaskTimePare.Tim1min_flag=0;
}


//===========================================================================
// No more.
//===========================================================================
