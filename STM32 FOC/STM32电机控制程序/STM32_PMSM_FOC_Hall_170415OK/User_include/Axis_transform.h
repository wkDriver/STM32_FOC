//############################################################
// FILE:  Axis_transform.h
// Created on: 2017年1月5日
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

#ifndef Axis_transform_H
#define Axis_transform_H

#include "stm32f10x.h"
#include "IQ_math.h"

typedef struct {  
	        int32_t  As;  		//  三相电流A
				  int32_t  Bs;			//  三相电流B
				  int32_t  Cs;			//  三相电流C
				  int32_t  Alpha;		//  二相静止坐标系 Alpha 轴
				  int32_t  Beta;		//  二相静止坐标系 Beta 轴
		 	 	} CLARKE ,*p_CLARKE ;

#define  CLARKE_DEFAULTS {0,0,0,0,0}  // 初始化参数

typedef struct {  
	        int32_t  Alpha;  	//  二相静止坐标系 Alpha 轴
		 		  int32_t  Beta;	 	//  二相静止坐标系 Beta 轴
		 	 	  int32_t  Angle;		//  电机磁极位置角度0---65536即是0---360度 
		 	 	  int32_t  Ds;			//  电机二相旋转坐标系下的d轴电流
		 	 	  int32_t  Qs;			//  电机二相旋转坐标系下的q轴电流
		 	 	  int32_t  Sine;    //  正弦参数，-32768---32767  -1到1 
		 	 	  int32_t  Cosine;  //  余弦参数，-32768---32767  -1到1
		 	 	} PARK , *p_PARK ;

#define  PARK_DEFAULTS {0,0,0,0,0,0,0}  // 初始化参数

typedef struct {  
	        int32_t  Alpha;  		// 二相静止坐标系 Alpha 轴
		 	 	  int32_t  Beta;		  // 二相静止坐标系 Beta 轴
		 	 	  int32_t  Angle;		  // 电机磁极位置角度0---65536即是0---360度  
		 	 	  int32_t  Ds;			  //  电机二相旋转坐标系下的d轴电流
		 	 	  int32_t  Qs;			  //  电机二相旋转坐标系下的q轴电流
		 	    int32_t  Sine;		  //  正弦参数，-32768---32767  -1到1 
		 	    int32_t  Cosine;		//  余弦参数，-32768---32767  -1到1
		 	    } IPARK , *p_IPARK;

#define  IPARK_DEFAULTS {0,0,0,0,0,0,0}  // 初始化参数

void  CLARKE_Cale(p_CLARKE  pV); // 三相到二相变换 克拉克变换
void  PARK_Cale(p_PARK pV)	 ;   // 二相到二相变换 怕克变换
void  IPARK_Cale(p_IPARK pV) ;   // 二相到二相变换反怕克变换

#endif /* Axis_transform*/
//===========================================================================
// End of file.
//===========================================================================
