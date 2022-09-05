//############################################################
// FILE: VF_angle.h
// Created on: 2017年1月15日
// Author: XQ
// summary: VF_angle
// VF  
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
#ifndef  VF_angle_H
#define  VF_angle_H
 
#include "IQ_math.h"
#include "stm32f10x.h"

typedef struct {
	    int32_t      SpeedRef ;    // 给定参考速度
	    int32_t      Speed_target ; // 实际目标速度
	    int32_t      Speederror ;   // 实际目标速度和参考速度差
	    int32_t      step_Speed ;   // 步进速度
	    int32_t      step_angle ;   // 步进角度
	    int32_t      step_anglemax ;// 最大步进角
	    uint16_t     BASE_FREQ ;    //电机的基本频率
	    int32_t      Angle_theta ;  //角度值
	    int32_t      Delta_theta ;   //角度变化
	    int32_t      Delta_thetaF;   //角度变化滤波值
	    uint16_t     Poles ;         //电机极对数
	    int32_t      VF_Vd;          // VF给定矢量d轴电压
	    int32_t      VF_Vq;          // VF给定矢量q轴电压
	   }VF_Angle;

#define  VF_angle_DEFAULTS   {0,0,0,10,0,0,240,0,0,4,0,0}  // 初始化参数

void  VF_start_control(void); // VF的变化控制
void  VFAngle_init(void );  // VF的初始化函数
void  VFAngle_cale(void );  // VF的根据频率自加减角度

#endif /* VF_angle_H*/
