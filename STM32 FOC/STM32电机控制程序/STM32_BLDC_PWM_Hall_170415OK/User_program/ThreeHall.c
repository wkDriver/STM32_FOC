//############################################################
// FILE: ThreeHall.c
// Created on: 2017年1月18日
// Author: XQ
// summary: ThreeHall
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
#include "ThreeHall.h"
#include "Tim1_PWM.h"
#include "Task_function.h"

extern   Hall   Hall_Three;
extern  logic   logicContr;

  uint16_t   HallK1=355; 		//滤波系数K1: 0.05/(T+0.05);
  uint16_t   HallK2=669 ;  	//滤波系数 K2: T/(T+0.05);
 
 void  ThreeHallPara_init(void )
{
   Hall_Three.Poles=4;
	 Hall_Three.speed_coeff= 65536/Hall_Three.Poles; // 2毫秒计算一次角度差值 1000/2ms=500   =7500
}


void ThreeHall_huanxkz(void)  // 一个PWM周期执行一次
{
	   if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1) 
     Hall_Three.HallUVW[0]=1;
		 else
		 Hall_Three.HallUVW[0]=0; 
   	 if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1) 
     Hall_Three.HallUVW[1]=1;
		 else
		 Hall_Three.HallUVW[1]=0; 
		 if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==1) 
     Hall_Three.HallUVW[2]=1;
		  else
		 Hall_Three.HallUVW[2]=0;  
	  Hall_Three.Hall_State = Hall_Three.HallUVW[0] +(Hall_Three.HallUVW[1]<<1) +(Hall_Three.HallUVW[2]<<2);
 if(  logicContr.Start_order==1  )
  {
    if ( Hall_Three.Hall_State!=Hall_Three.OldHall_State )
      {
     	   switch (Hall_Three.Hall_State )
	      {
					case  Hall_num1:     // 0x5
					 {
					 if( logicContr.Run_mode==1)
						MOS_Q63PWM();
						if( logicContr.Run_mode==2)
						MOS_Q54PWM();
					 }
					break;
					case  Hall_num2:      // 0x1
					 {
					if( logicContr.Run_mode==1)
						MOS_Q32PWM();
					if( logicContr.Run_mode==2)
						MOS_Q41PWM();
					 }
					break;
					case  Hall_num3:     //0x3
					 {
						if( logicContr.Run_mode==1)
						MOS_Q25PWM();
						if( logicContr.Run_mode==2)
						MOS_Q16PWM();
					 }
					 break;
					 case Hall_num4:    // 0x2
					 {
						 if( logicContr.Run_mode==1)
						  MOS_Q54PWM();
						 if( logicContr.Run_mode==2)
							MOS_Q63PWM();
						}
					 break;
					 case Hall_num5:    // 0x6
					 {
						 if( logicContr.Run_mode==1)
					  	MOS_Q41PWM();
						 if( logicContr.Run_mode==2)
							MOS_Q32PWM();
					 }
					 break;
					 case Hall_num6:   // 0x4
						{
						if( logicContr.Run_mode==1)
							MOS_Q16PWM();
						if( logicContr.Run_mode==2)
							MOS_Q25PWM();
						}
					 break;
						 default:
						 {
							Stop_Motor();
							Hall_Three.Speed_RPM=0;
						 }
					 break;
	    	 }		
		  Hall_Three.Speed_countFitter= _IQ10mpy(HallK2, Hall_Three.Speed_countFitter)+_IQ10mpy(HallK1,  Hall_Three.Speed_count);								  	 
      Hall_Three.Speed_RPM = Hall_Three.speed_coeff/Hall_Three.Speed_countFitter;
		  Hall_Three.Speed_RPMF= _IQ10mpy(HallK2, Hall_Three.Speed_RPMF)+_IQ10mpy(HallK1,  Hall_Three.Speed_RPM);	
  		Hall_Three.Speed_count= 0;        
      }

     else  if ( Hall_Three.Hall_State==Hall_Three.OldHall_State )
     {
    	 Hall_Three.Speed_count++;   
       if( Hall_Three.Speed_count>=2000 )
    	 {
    		 Hall_Three.Speed_count=0;
    		 Hall_Three.Speed_RPMF= 0;
				 Stop_Motor(); 
    		 Hall_Three.step_angleFitter=0;	
				 Hall_Three.Move_State=0;	
			   switch (Hall_Three.Hall_State )
					{
				   case Hall_num1:  //  0x5
						{
						if( logicContr.Run_mode==1)
							 MOS_Q63PWM();
						if( logicContr.Run_mode==2)
							 MOS_Q54PWM();
						}
						break;
				   case  Hall_num2:   //0x1
						{
						if( logicContr.Run_mode==1)
							MOS_Q32PWM();
						if( logicContr.Run_mode==2)
							MOS_Q41PWM();
						}
					 break;
				   case Hall_num3:   // 0x3
						{
							if( logicContr.Run_mode==1)
							MOS_Q25PWM();
							if( logicContr.Run_mode==2)
							MOS_Q16PWM();
						}
						break;
				    case  Hall_num4: // 0x2
						{
							 if( logicContr.Run_mode==1)
							MOS_Q54PWM();
							 if( logicContr.Run_mode==2)
								MOS_Q63PWM();
						}
					  break;
				    case  Hall_num5:  //0x6
						{
						 if( logicContr.Run_mode==1)
							MOS_Q41PWM();
						 if( logicContr.Run_mode==2)
								MOS_Q32PWM();
						}
						break;
				    case  Hall_num6:   // 0x4
						{
						if( logicContr.Run_mode==1)
						 MOS_Q16PWM();
						if( logicContr.Run_mode==2)
						 MOS_Q25PWM();
						}
					 break;	 
					}	 
       }				 
     }
	 }		 
   Hall_Three.OldHall_State=Hall_Three.Hall_State ;
}

 
//===========================================================================
// No more.
//===========================================================================
