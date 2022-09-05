#ifndef _SVPWM_ICS_H
#define _SVPWM_ICS_H


void SVPWM_IcsInit(void);
Curr_Components SVPWM_IcsGetPhaseCurrentValues(void);
void SVPWM_IcsCalcDutyCycles (Volt_Components Stat_Volt_Input);
void SVPWM_IcsCurrentReadingCalibration(void);
u8 SVPWMEOCEvent(void);

#endif