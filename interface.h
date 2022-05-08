#ifndef __INTERFACE
#define __INTERFACE

#include "stm32f1xx_hal.h"


int button(uint16_t usC, uint16_t usP, int width, int height, const char *pstr, int wordCount, uint16_t color, uint16_t wordColor);
int testInterface(void);
void touch2Display(int16_t *x, int16_t *y);
void loading(void);
int mainMenu(void);
int timerScreen(void);
void bigNumber(uint16_t usC, uint16_t usP, int num);
int timerCount(int timer);
int checkButton(int mode);

#endif
