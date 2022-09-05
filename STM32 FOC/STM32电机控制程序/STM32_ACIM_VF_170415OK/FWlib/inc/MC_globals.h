#ifndef _MC_globals_h
#define _MC_gloabals_h

#include "stm32f10x.h"
#include "MC_type.h"
#include "MC_const.h"

#define HALL_MAX_SPEED               ((u16)5000) // Unit is 0.1Hz


/*Variable of convenience*/
extern Curr_Components Stat_Curr_a_b;              /*Stator currents Ia,Ib*/ 

extern Curr_Components Stat_Curr_alfa_beta,Stat_Curr_alfa_beta1;        /*Ialpha & Ibeta, Clarke's  
                                                  transformations of Ia & Ib */

extern Curr_Components Stat_Curr_q_d,Stat_Curr_q_d1;         /*Iq & Id, Parke's transformations
                                                of Ialpha & Ibeta, */

extern Volt_Components Stat_Volt_a_b;              /*Stator voltages Va, Vb*/ 

extern Volt_Components Stat_Volt_q_d;         /*Vq & Vd, voltages on a reference
                                          frame synchronous with the rotor flux*/

extern Volt_Components Stat_Volt_alfa_beta;       /*Valpha & Vbeta, RevPark
                                                    transformations of Vq & Vd*/

extern volatile u32 wGlobal_Flags;

extern volatile SystStatus_t State;

extern PID_Struct_t       PID_Torque_InitStructure;
extern PID_Struct_t       PID_Flux_InitStructure;
extern PID_Struct_t       PID_Speed_InitStructure;
extern PID_Struct_t       PID_Current_InitStructure;
extern volatile s16 hFlux_Reference;
extern volatile s16 hTorque_Reference;
extern volatile s16 hSpeed_Reference;
extern volatile s16 Stat_Threshold,Stat_Curr_q_d_Ref,Stat_Curr_q_d_Threshold;

extern volatile SystStatus_t State;
extern volatile u16 phase_A,phase_B,Stall_Counter,wAvrgBuffer;
extern volatile u8 Uart_Data1,Uart_Data2,Uart_Data3,Uart_Data4,Counter2,Closeloop_Flag;
extern volatile s16 RotorFreq_dpp,Rotor_Speed,RotorSpeed_Avrg,Measure_Angle1,Measure_Angle2,pre_angle;
extern volatile s16 SpeedMesAux[];
extern volatile s16 Temp_Data[];
extern volatile s8 bSpeed_State;
extern volatile s16 Stat_Curr_q_d_Ramp;
extern volatile u32 hSpeedHall_2;
#endif


