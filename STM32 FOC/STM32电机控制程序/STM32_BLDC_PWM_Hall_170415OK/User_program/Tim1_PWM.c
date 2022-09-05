//############################################################
// FILE: Tim1_PWM.c
// Created on: 2017年1月15日
// Author: XQ
// summary: Tim1_PWM
// 定时器1电机控制，正交编码器的定时器IO    
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

#include "Tim1_PWM.h"

int16_t  DUTY=0,PERIOD=2880;

void  Tim1_PWM_Init(void)
{ 
  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
  TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable TIM1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
 
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       //  模式  增
  TIM_TimeBaseStructure.TIM_Period = 5999;                    //    从装比较最大   18k  =   72000/ 6000= 12k   
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 //  分频
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;      
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;                               //  控制占空比
  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ;         //  上桥臂
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High ;        //  下桥臂
	
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);                         //  比较单元 1 2 3 
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);               //  
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
  
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  /* The DeadTime=8(32+DTG[4:0])Tdts 0xC4=>DeadTime=4us */                 
  TIM_BDTRInitStructure.TIM_DeadTime = 0xC4;                       //  带死区   4us  DTG= C4
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);	
  
	TIM_ClearITPendingBit(TIM1, TIM_IT_Break);  //清中断标志位
  TIM_ITConfig(TIM1,TIM_IT_Break ,ENABLE); //使能中断 
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清中断标志位
  TIM_ITConfig(TIM1,TIM_IT_Update ,ENABLE); //打开中断 
 
  TIM_Cmd(TIM1, ENABLE);//计数开始
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

/* Configure one bit for preemption priority */
	//选择优先级分组
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//配置TIM1的刹车中断使能
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//指定抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//指定抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//配置TIM1的更新中断使能
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//指定抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//指定响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void Stop_Motor(void)
{
   TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
	
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);
  
   TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
}

void MOS_Q41PWM(void)      // 61   ??     5  4  6   2  3   1     Q63  Q32  Q25  Q54  Q41  Q16
{   
	 TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
	
   TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
	  
	  TIM_SetCompare1(TIM1, DUTY );
    TIM_SetCompare2(TIM1, 0);
	
}
 
void MOS_Q16PWM(void)
{    
	 TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
	
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

	 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1); 
	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
	 TIM_SetCompare1(TIM1, DUTY );
   TIM_SetCompare3(TIM1, 0);
}
 
void MOS_Q63PWM(void)
{    
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
	
	 TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

	 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
	 TIM_SetCompare2(TIM1, DUTY );
   TIM_SetCompare3(TIM1, 0);
}
 
void MOS_Q32PWM(void) //
{    
	 TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1); 
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
	
	 TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable); 
	 TIM_SetCompare1(TIM1, 0);	
 	 TIM_SetCompare2(TIM1, DUTY );   
}


void MOS_Q25PWM(void)
{
	 TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_PWM1); 
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

	 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);	
	  TIM_SetCompare1(TIM1, 0 );
    TIM_SetCompare3(TIM1, DUTY);	
}
 
void MOS_Q54PWM(void)
{  
	 TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
 
	 TIM_SelectOCxM(TIM1, TIM_Channel_2, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
  
 
	 TIM_SelectOCxM(TIM1, TIM_Channel_3, TIM_OCMode_PWM1);
	 TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	 TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);	
	 TIM_SetCompare2(TIM1, 0 );
   TIM_SetCompare3(TIM1, DUTY);
}
 
//===========================================================================
// No more.
//===========================================================================
