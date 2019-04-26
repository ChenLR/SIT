#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

void USART2_Init(void);
void USART2_PutChar(char c);
void USART2_PutString(char *s);
void USART2_PutInt(int32_t num);
void USART2_PutFloat(float f);

#endif
