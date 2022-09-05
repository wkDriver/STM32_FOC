//############################################################
// FILE: Tim4_Encoder_PWMDAC.h
// Created on: 2017年1月18日
// Author: XQ
// summary: Tim4_Encoder_PWMDAC
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
   
#ifndef _Tim4_Encoder_PWMDAC_H
#define _Tim4_Encoder_PWMDAC_H 

#include "IQ_math.h"
#include "stm32f10x.h"

typedef struct {	    
	    int32_t  ElecThetaYS;      // 编码器电机位置角度电角度 0---1024 0--360
	    int32_t  ElecTheta;        // 电机的磁极电角度 0---65536 0--360   Electrical angle
	    int32_t  MechTheta;        // 编码器电机位置机械角度 0--4096   Mechanical Angle	   
	    int32_t  JZElecTheta;      // 矫正电机磁极位置角度 
	    uint16_t Poles;            //   Number of pole pairs  	  
	    int32_t  initial_angle;    //  电机初始位置角度   
      int32_t  Oid_JZElecTheta;  //  矫正后历史电机磁极位置角度 
      int32_t	 Speed_ele_angleIQ;//用于速度计算的电角度
	    int32_t  Speed_ele_angleIQFitter;// 滤波后速度电角度
		  uint16_t Speed_RPM;     // 电机速度RPM  		 
	    uint32_t speed_coeff;  // 计算速度的系数 
    	uint16_t SpeedK1;    // 速度计算一阶低通滤波器系数1
	    uint16_t SpeedK2;    // 速度计算一阶低通滤波器系数2
   } EQEP;

#define  EQEP_DEFAULTS  { 0,0,0,0,4,0,0,0,0,0,0,364,660} // 初始化参数
 
void TIM4_Encoder_Config(void); //定时器4初始化的编码器配置
void EXTIX_Init(void);          // Z信号编码器配置外部中断
void QEPEncoder_Cale(void);     //编码器电机角度计算
void EQEP_Pare_init(void );     //编码器参数初始化
void QEPEncoder_Speedcale(void);//编码器速度计算
#endif /* __Tim4_Encoder_PWMDAC_H */

//===========================================================================
// No more.
//===========================================================================
