#pragma once
#include "myDefines.h"

extern uint8_t segments[];
extern uint16_t LEDSequence[8];
extern GPIO_My portSequence[8];

void My_Clock_Init(void);
void Display_Init(void);
void JoyStick_Init(void);
int Check_Input(void);
void Diodes_Init(void);
void Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode);
void My_Delay(int miliseconds);
//void Display(int number);
void setRGBLed(char* ledColor);
void resetRGBLed(char* ledColor);
void toggleRGBLed(char* ledColor);
int statusRGBLed(char* ledColor);

void setLed(uint16_t ledNumber);
void resetLed(uint16_t ledNumber);
void toggleLed(uint16_t ledNumber);
int getLedStatus(uint16_t ledNumber);

int statusJoy(char* JoyPosition);

void displayNumber(int number);
void blinkLedInMain(int* blinkCounter, char** currentlyBlinkingLed);
