#pragma once
#include "myDefines.h"

void My_Clock_Init(void);
void Display_Init(void);
void JoyStick_Init(void);
int Check_Input(void);
void Diodes_Init(void);
void Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode);
void My_Delay(int miliseconds);
