//############################################################
// FILE:  Sensorless_SMO.h
// Created on: 2017年1月18日
// Author: XQ
// summary: Sensorless_SMO
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

#ifndef  Sensorless_SMO_H
#define  Sensorless_SMO_H

#include "IQ_math.h"
#include "stm32f10x.h" 
#include "math.h"
 

typedef struct {
	     int32_t  Valpha;       //二相静止坐标系alpha-轴电压	
	     int32_t  Ealpha;       //二相静止坐标系alpha-轴反电动势
	     int32_t  Zalpha;       //alfa轴滑膜观测器的z平面
	     int32_t  Gsmopos;      //滑膜常数1
	     int32_t  EstIalpha;    //滑膜估算alpha-轴电流
	     int32_t  Fsmopos;      //滑膜常数2
	     int32_t  Vbeta;        //二相静止坐标系beta-轴电压	 
	     int32_t  Ebeta;  	    //二相静止坐标系beta-轴反电动势
	     int32_t  Zbeta;        //beta轴滑膜观测器的z平面	 
	     int32_t  EstIbeta;     //滑膜估算beta-轴电流 
	     int32_t  Ialpha;  	    //二相静止坐标系alpha-轴电流
	     int32_t  IalphaError;  //二相静止坐标系beta-轴电流误差
	     int32_t  Kslide;       //滤波器系数	 
	     int32_t  Ibeta;  	    //二相静止坐标系beta-轴电流 
	     int32_t  IbetaError;   //二相静止坐标系beta-轴电流误差
	     int32_t  Kslf;         //滤波器系数
	     int32_t  E0;	          //滑膜的电流误差的限幅值 0.5
	   } Angle_SMO, *p_Angle_SMO;

#define Angle_SMO_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0} // 初始化参数

typedef struct {
	    int32_t  Speed_ele_angleIQ;  //速度电角度值（计算速度）  
	    int32_t  old_ele_angleIQ;   // 电机历史电角度
	    int32_t  ele_angleIQ;      // 电机电角度
	    int32_t  Speed_ele_angleIQFitter;  //速度电角度值（计算速度）    	 
	    uint16_t Speed_RPM;       	 //电机旋转速度 	 
	    uint32_t speed_coeff;       //计算速度的系数
    	    uint16_t SpeedK1;           // 速度滤波系数K1
	    uint16_t SpeedK2;         // 速度滤波系数K2
	   }Speed_est;

#define Speed_est_DEFAULTS {0,0,0,0,0,0,0,0} // 初始化参数

typedef struct{
	  float  Rs; 			//电机的相电阻	 
	  float  Ls;			//电机的相电感	  
    float  Ib; 			//电机控制器的基本相电流 	  
	  float  Vb;			//电机控制器的基本相电压	 
    float  Ts;			 //采样周期	 
    uint32_t   POLES; // 电机的极对数
    uint32_t   BASE_FREQ; // 电机控制器的基本频率
	  float  Fsmopos;		   //滑膜常数1
    float  Gsmopos;			 //滑膜常数2
  }SMO_Motor;

#define SMO_Motor_DEFAULTS {0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0} // 初始化参数

void  Angle_Cale(p_Angle_SMO  pV); //滑膜电机位置电角度计算
void  SMO_Pare_init (void );   // 滑膜观测器的参数初始化
void SMO_Speedcale(void) ;    //  滑膜的角度计算速度函数
#endif /* Sensorless_SMO*/
