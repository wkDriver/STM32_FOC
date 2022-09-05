//############################################################
// FILE:    Svpwm_dq.h
// Created on: 2017年1月15日
// Author: XQ
// summary: Header file  and definition
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

#ifndef  Svpwm_dq_H
#define  Svpwm_dq_H

#include "stm32f10x.h"
#include "IQ_math.h"

typedef struct 	{ 
	        int32_t  Ualpha; 		//  二相静止坐标系alpha-轴
				  int32_t  Ubeta;	//  二相静止坐标系beta-轴 
				  int32_t  Ta;		//  三相矢量占空比Ta
				  int32_t  Tb;		//  三相矢量占空比Tb
				  int32_t  Tc;		//  三相矢量占空比Tc
				  int32_t  tmp1;	//  三相静止坐标系的电压temp1   
				  int32_t  tmp2;	//  三相静止坐标系的电压temp2
				  int32_t  tmp3;	//  三相静止坐标系的电压temp3
				  uint16_t VecSector;	// 矢量空间扇区号
				} SVPWM , *p_SVPWM ;

#define SVPWM_DEFAULTS  { 0,0,0,0,0,0,0,0,0}  // 初始化参数

void  SVPWM_Cale(p_SVPWM pV);

#endif /* Svpwm_dq*/
