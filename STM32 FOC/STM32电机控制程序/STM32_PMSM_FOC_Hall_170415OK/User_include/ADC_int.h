//############################################################
// FILE:  ADC_int.h
// Created on: 2017年1月9日
// Author: XQ   
// summary: Header file  and definition
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

#ifndef ADC_int_H
#define ADC_int_H

#include "stm32f10x.h"
 
typedef struct {
	     int32_t   BUS_Curr ;     // 母线电流 DC Bus  Current
	     int32_t   PhaseU_Curr;   // U相电流 Phase U Current
	     int32_t   PhaseV_Curr;   // V相电流Phase V Current
	     int32_t   BUS_Voltage ;  //母线电压DC Bus  Voltage	     
	     int32_t   RP_speed_Voltage ;   // 电位器电压 RP1_Voltage
	     int32_t   OffsetBUS_Curr ;     // 母线电流偏执值 DC Bus  Current
	     int32_t   OffsetPhaseU_Curr;   // U相电流偏执值  Phase U Current
	     int32_t   OffsetPhaseV_Curr;   // V相电流偏执值 Phase V Current
	     int32_t   Coeff_filterK1;   // 一阶低通滤波器系数1
		   int32_t   Coeff_filterK2;   // 一阶低通滤波器系数2
       }ADCSamp;

#define  ADCSamp_DEFAULTS  {0,0,0,0,0,0,0,0,268,756}   // 初始化参数

void  Offset_CurrentReading(void);  //偏执电压读取
void  DMA_Configuration(void);      //DMA配置
void  ADC1_Configuration(void);     //ADC1配置
void  ADC_Sample(void );            //采样函数
#endif  // end of ADC_int_H definition

//===========================================================================
// End of file.
//===========================================================================
