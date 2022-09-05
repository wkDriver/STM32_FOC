//############################################################
// FILE: Tim1_PWM.h
// Created on: 2017年1月18日
// Author: XQ
// summary: Tim1_PWM
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
   
#ifndef _Tim1_PWM_H
#define _Tim1_PWM_H 

#include "IQ_math.h"
#include "stm32f10x.h"

#define CKTIM	((u32)72000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */
#define PWM_PRSC ((u8)0)                          
#define PWM_PERIOD ((u16) (CKTIM / (u32)(2 * PWM_FREQ *(PWM_PRSC+1)))) //72000000/(2*12500*(1))=2880  2880=PWM 100%
/****	Power devices switching frequency  ****/

#define PWM_FREQ ((u16)12500) // in Hz  (N.b.: pattern type is center aligned),不是中央对齐时12.5kHZ，中央对齐模式是
#define PWM_HalfPerMax   ((u16)1440)
#define REP_RATE (0)  
 // (N.b): Internal current loop is performed every (REP_RATE + 1)/(2*PWM_FREQ) seconds.               
 // REP_RATE has to be an odd number in case of three-shunt
 // current reading; this limitation doesn't apply to ICS
#define DEADTIME  (u16)((unsigned long long)CKTIM/2*(unsigned long long)DEADTIME_NS/1000000000uL)
#define DEADTIME_NS	((u16) 1800)  //in nsec; range is [0...3500]  

#define DCBUS_VOLTAGE			(int32_t)_IQ(24)		//   Q10格式,母线电压24V  24576 

#define STATOR_VOLTAGE		 (int32_t)_IQ(13.867) 		// Udc/√3* Q15  13.867   14190  格式   

#define MAX_STATOR_VOLTAGE	(int32_t)_IQ(13.5)  	 // Udc/√3*0.97 13.467   13765 Q15格式  

void Svpwm_Outpwm(void);  //SVPWM输出函数
void Tim1_PWM_Init(void); // 高级定时器1函数
void Stop_Motor(void);    //电机停止函数关PWM
void Start_Motor(void);   //电机开始函数开PWM
#endif /* __TIMER_H */

//===========================================================================
// No more.
//===========================================================================
