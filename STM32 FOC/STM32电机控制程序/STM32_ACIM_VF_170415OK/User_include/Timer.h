//############################################################
// FILE: Timer.h
// Created on: 2017年1月18日
// Author: XQ
// summary: Timer_ 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//DSP/STM32电机控制开发板
//硕历电子
//网址: https://shuolidianzi.taobao.com
//修改日期:2017/1/23
//版本：V17.3-1
//Author-QQ: 616264123
//电机控制QQ群：314306105
//############################################################
   
#ifndef _TIMER_H
#define _TIMER_H 

#include "stm32f10x.h"

typedef struct {	
	      uint8_t   PWMZD_count;	  // 在PWM中计数  1/12.5K *25=2ms
        uint8_t   IntClock_10ms; 	// 10ms时钟标志
        uint8_t   Tim10ms_flag;   // 10ms标志
        uint8_t   Tim100ms_count; //100ms计数
        uint8_t   Tim100ms_flag;  //100ms事件标志位
        uint8_t   Tim500ms_count; //500ms计数
        uint8_t   Tim500ms_flag; //500ms事件标志位
        uint8_t   Tim1s_count;  //1s计数
        uint8_t   Tim1s_flag ;  //1s事件标志位
        uint8_t   Tim10s_count; //10s计数
        uint8_t   Tim10s_flag ; //10s事件标志位
        uint8_t   Tim1min_count; //1计数
        uint8_t   Tim1min_flag ; //1分钟事件标志位
	   }TaskTime;

#define  TaskTime_DEFAULTS  {0,0,0,0,0,0,0,0,0,0,0,0,0}  // 初始化参数
   
void SysTickConfig(void);  // 滴答定时器
void RunSystimer(void);    // 运行计数事件标志
void CLEAR_flag(void);     // 清除事件标志位

#endif /* __TIMER_H */

//===========================================================================
// No more.
//===========================================================================
