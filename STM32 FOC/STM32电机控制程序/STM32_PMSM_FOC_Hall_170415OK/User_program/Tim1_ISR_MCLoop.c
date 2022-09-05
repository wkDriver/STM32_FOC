
#include "Tim1_ISR_MCLoop.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"
#include "GPIO_int.h"
#include "ThreeHall.h"
#include "Axis_transform.h"
#include "Svpwm_dq.h"
#include "Task_function.h"
#include "PI_Cale.h"
#include "Timer.h"
#include "Usart_RS232.h"
#include "Tim4_Encoder_PWMDAC.h"
 
extern  PI_Control   pi_spd ;
extern  PI_Control   pi_id  ;
extern  PI_Control   pi_iq  ;
extern   ADCSamp    ADCSampPare;
extern   uint16_t   PWM_DUTY[3] ;
extern   Hall       Hall_Three;
extern   logic      logicContr;
extern   TaskTime   TaskTimePare;  
extern   CLARKE     ClarkeI;
extern   PARK       ParkI;
extern   IPARK      IparkU;
extern   SVPWM      Svpwmdq;
extern   IQSin_Cos  AngleSin_Cos;
extern   Test       TestPare;
extern   IQAtan     IQAtan_Pare;
 
uint16_t  FilK1=328;
uint16_t  FilK2=696;

void TIM1_UP_IRQHandler(void)// 触发ADC中断采样和电机环路控制
{
	//此程序更新中断一直执行
	 TIM_ClearFlag(TIM1, TIM_FLAG_Update); // 清除标志位
	 
   ThreeHallanglecale( );  //霍尔状态和计算位置角度
	 TaskTimePare.PWMZD_count++;
   
	 if(TaskTimePare.PWMZD_count==25 ) // 2ms的事件任务
	 {
	   TaskTimePare.PWMZD_count=0;
	   Hall_Three_Speedcale( );  //根据霍尔角度计算速度
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
     knob_control( );   //转把电位器调速的控制
 	
		 TestPare.id_test=  (pi_id.Fbk>>3 );   //通讯测试参数输出
	   TestPare.iq_test=  (pi_iq.Fbk>>3 );
	   TestPare.ud_test=  (IparkU.Ds>>3 );
	   TestPare.uq_test=  (IparkU.Qs>>3 );
     // 测试参数输出	
     TestPare.fact_BUS_Voil= ADCSampPare.BUS_Voltage;
	   TestPare.fact_BUS_Curr= ADCSampPare.BUS_Curr;
	   TestPare.Speed_fact= ( pi_spd.Fbk>>3);
	   TestPare.Speed_target=( pi_spd.Ref>>3);	
 	 
	   pi_spd.Fbk = Hall_Three.Speed_RPM ;  //   0---4096RPM
	   PI_Controller((p_PI_Control)&pi_spd); // 速度环PI控制 
	   pi_spd.OutF= _IQ10mpy(FilK1,pi_spd.OutF)+_IQ10mpy(FilK2,pi_spd.Out); //一阶低通滤波器	 
   }
	
	  ADC_Sample( );  // 相电流母线电流电压采集
	 
  	ClarkeI.As=ADCSampPare.PhaseU_Curr;
	  ClarkeI.Bs=ADCSampPare.PhaseV_Curr;

	  CLARKE_Cale((p_CLARKE)&ClarkeI ); // CLARKE变换

	  ParkI.Alpha=ClarkeI.Alpha;
	  ParkI.Beta=ClarkeI.Beta;
 	  
	  AngleSin_Cos.IQAngle= Hall_Three.ele_angleIQ;
 
	  IQSin_Cos_Cale((p_IQSin_Cos)&AngleSin_Cos); //磁极位置角度，正余弦计算函数

		ParkI.Sine =AngleSin_Cos.IQSin;  
	  ParkI.Cosine =AngleSin_Cos.IQCos;   

	  PARK_Cale((p_PARK)&ParkI);  // park变换
	
    pi_id.Ref = 0;
	  pi_iq.Ref= pi_spd.OutF;

	  pi_id.Fbk = ParkI.Ds;
	  PI_Controller((p_PI_Control)&pi_id);  // id轴 PI控制
	  pi_id.OutF= _IQ10mpy(FilK1,pi_id.OutF)+_IQ10mpy(FilK2,pi_id.Out);

	  pi_iq.Fbk = ParkI.Qs;
	  PI_Controller((p_PI_Control)&pi_iq);  // iq轴 PI控制
	  pi_iq.OutF= _IQ10mpy(FilK1,pi_iq.OutF)+_IQ10mpy(FilK2,pi_iq.Out);
 
	  if(logicContr.Run_mode==1)
	  {
	    IparkU.Ds=0;   // 采用开环
	    IparkU.Qs= 10*pi_spd.Ref;  //
	  }

	  if(logicContr.Run_mode==2)
	  {
	  	IparkU.Ds= pi_id.OutF;   // 采用id电流闭环
	  	IparkU.Qs= pi_spd.OutF;  // 采用速度闭环   
	  }
    if(logicContr.Run_mode==3)
	  {
       IparkU.Ds= -pi_id.OutF;   //反转控制 ，id电流闭环
       IparkU.Qs= -pi_spd.OutF;  // 采用速度闭环 
    }
    if(logicContr.Run_mode==4)
    {
			 IparkU.Ds= pi_id.OutF;   // 采用速度闭环  id电流闭环
			 IparkU.Qs= pi_iq.OutF;   // 1=2^15=32768 最大值
    }
        
    IparkU.Sine=AngleSin_Cos.IQSin;
    IparkU.Cosine=AngleSin_Cos.IQCos;
	  IPARK_Cale((p_IPARK)&IparkU);      // 反park变换
 	
	  Svpwmdq.Ualpha =IparkU.Alpha;
	  Svpwmdq.Ubeta = IparkU.Beta;

    SVPWM_Cale((p_SVPWM)&Svpwmdq);    // SVPWM计算占空比
 
	  Svpwm_Outpwm( );  // PWM输出  
		 
    TIM_SetCompare3(TIM4 , (Hall_Three.ele_angleIQ>>5));  //DAC通道1    
	  TIM_SetCompare4(TIM4 , PWM_DUTY[0]);                  //DAC通道2 
} 


//===========================================================================
// No more.
//===========================================================================
