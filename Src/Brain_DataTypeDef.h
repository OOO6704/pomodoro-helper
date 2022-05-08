#ifndef __BRAINDATATYPEDEF
#define __BRAINDATATYPEDEF

#include "stm32f1xx_hal.h"

typedef struct
{
    uint8_t Brain_Data[36];
    uint8_t signal;
    uint8_t attention;
    uint8_t relax;
    volatile uint8_t receive_flag;
    volatile uint8_t wear_flag;
    volatile uint8_t off_flag;
    volatile uint8_t mode;
    volatile uint32_t LowAlpha;
    volatile uint32_t HighAlpha;
    volatile uint32_t LowBeta;
    volatile uint32_t HighBeta;

}Brain_DataTypeDef;

extern Brain_DataTypeDef Brain_DataStruct;

#endif
