#ifndef _MC_TYPE_H
#define _MC_TYPE_H

//#include "stm32f10x.h"


#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//上面的定义在2.0的库中在stm32f10x_type.h中

typedef struct
{
 s16 qI_Component1;
  s16 qI_Component2;
}Curr_Components;

typedef struct 
{
  s16 qV_Component1;
  s16 qV_Component2;
} Volt_Components;

typedef struct
{
  s16 hCos;
  s16 hSin;
} Trig_Components;

typedef struct
{
 s16 hC1;
 s16 hC2;
 s16 hC3;
 s16 hC4;
 s16 hC5;
 s16 hC6;
 s16 hF1;
 s16 hF2;
 s16 hF3;
 s16 PLL_P;
 s16 PLL_I;
 s32 wMotorMaxSpeed_dpp;
 u16 hPercentageFactor;
} StateObserver_Const; //这个用不到

typedef struct
{
  s16 PLL_P;
  s16 PLL_I;
  s16 hC2;
  s16 hC4;
} StateObserver_GainsUpdate;//这个用不到

typedef struct
{
 s16 hsegdiv;
 s32 wangc[8];
 s32 wofst[8];
} MTPA_Const;//

typedef struct 
{  
  s16 hKp_Gain;
  u16 hKp_Divisor;
  s16 hKi_Gain;
  u16 hKi_Divisor;  
  s16 hLower_Limit_Output;     //Lower Limit for Output limitation
  s16 hUpper_Limit_Output;     //Lower Limit for Output limitation
  s32 wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wIntegral;
  // Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled in
  //stm32f10x_MCconf.h
  s16 hKd_Gain;
  u16 hKd_Divisor;
  s32 wPreviousError;
  s32 lastpioutput;
} PID_Struct_t;

typedef enum 
{
INIT, START, RUN, STOP, BRAKE, WAIT, FAULT
} SystStatus_t;

typedef enum 
{
NO_FAULT, OVER_VOLT, UNDER_VOLT
}BusV_t;

typedef enum 
{
NORMAL_CURRENT, OVER_CURRENT,UNDER_CURRENT
}BusCurrent;//自己定义的额


//




#endif

