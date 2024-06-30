#pragma once

typedef enum {
	INIT = 0,
	LED = 1,
	LED_DIS = 2,
	LPUART_LVL = 3,
	JOY = 4
} PossibleLevels;

int commandHandler(char* command, PossibleLevels* currentLevel, int* displayedNumber, char** currentlyBlinkingLed);

int menuNavigation(char* command, PossibleLevels* currentLevel);
void getHelp(char* command, PossibleLevels* currentLevel);

int handleLedCommands(char* firstWord, char* secondWord, char** currentlyBlinkingLed);
int handleLPUARTCommands(char* firstWord, char* secondWord);
int handleJoyCommands(char* firstWord, char* secondWord);
int handle7LEDCommands(char* firstWord, char* secondWord, int* displayedNumber);

void extractWords(char* command, char* firstWord, char* secondWord);

int isCorrectLEDCommand(char* firstWord, char* secondWord);
int isCorrectJoyCommand(char* firstWord, char* secondWord);
int isCorrectLPUARTCommand(char* firstWord, char* secondWord);
int isCorrect7LEDCommand(char* firstWord, char* secondWord);

void changeCurrentlyBlinkLED(char* secondWord, char** currentlyBlinkigLed);

void printWelcomeMessage();
void printMainMenuHelp();
void printLEDHelp();
void print7LEDHelp();
void printJoyHelp();
void printLPUARTHelp();


