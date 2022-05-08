#include "tone.h"

extern TIM_HandleTypeDef htim4;
extern void tone(int frequency, int time);


void Rickroll(){
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
HAL_Delay(100);
tone(196,125);
tone(220,125);
tone(261,125);
tone(220,125);
tone(329,325);
HAL_Delay(50);
tone(329,375);
tone(293,750);
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}

void mii(){
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
HAL_Delay(100);
tone(370,375);
HAL_Delay(125);
tone(440,250);
tone(554,500);
tone(440,500);
tone(370,250);
tone(294,188);
HAL_Delay(62);
tone(294,188);
HAL_Delay(62);	
tone(294,188);
HAL_Delay(1562);
	
tone(294,250);
tone(370,250);
tone(440,375);
HAL_Delay(125);
tone(370,250);
HAL_Delay(500);
tone(659,750);
tone(622,250);
tone(587,125);
HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
}
