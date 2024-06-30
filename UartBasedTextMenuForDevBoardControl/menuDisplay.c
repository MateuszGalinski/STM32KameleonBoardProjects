#include "menuDisplay.h"
#include "lpuart.h"
#include "fifo.h"
#include "usefulfunctions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int commandHandler(char* command, PossibleLevels* currentLevel, int* displayNumber, char** currentlyBlinkingLed){

	if(strcasecmp(command, "HELP") == 0){
		getHelp(command, currentLevel);
		Send_String("\n>");
		return 0;
	}
//-----------------------------------------------

	if(menuNavigation(command, currentLevel) /*returns 1 if command was navigation*/){
		Send_String("\n>");
		return 0;
	}

	char firstWord[BUFFERSIZE] = {0};
	char secondWord[BUFFERSIZE] = {0};

	extractWords(command, firstWord, secondWord);

	PossibleLevels curTemp = *currentLevel;
//	int handlerSuccess= 0;

	switch(curTemp){
		case INIT:
			return 1;
		case LED_DIS:
			if(isCorrect7LEDCommand(firstWord, secondWord)){
				handle7LEDCommands(firstWord, secondWord, displayNumber);
				Send_String("\n>");
				return 0;
			}
			return 1;
		case LED:
			if(isCorrectLEDCommand(firstWord, secondWord)){
				handleLedCommands(firstWord, secondWord, currentlyBlinkingLed);
				Send_String("\n>");
				return 0;
			}
			return 1;
		case LPUART_LVL:
			if(isCorrectLPUARTCommand(firstWord, secondWord)){
				handleLPUARTCommands(firstWord, secondWord);
				strcpy(command, "");
				Send_String("\n>");
				return 0;
			}
			return 1;
		case JOY:
			if(isCorrectJoyCommand(firstWord, secondWord)){
				handleJoyCommands(firstWord, secondWord);
				Send_String("\n>");
				return 0;
			}
			return 1;
	}

	return 1;

//-----------------------------------------------


	//return 1;

}


void extractWords(char* command, char* firstWord, char* secondWord) {
    char* token;

    token = strtok(command, " ");
    if (token != NULL) {
        strcpy(firstWord, token);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        strcpy(secondWord, token);
    }
}

void printWelcomeMessage(){
	Send_String("Simple menu by Mateusz Galinski\n");
	Send_String("Write 'help' to obtain more information\n");
	Send_String(">");
}

void printMainMenuHelp(){
	Send_String("Possible commands are:\n");
	Send_String("LED			goes to LED menu:\n");
	Send_String("7LED			goes to segment display menu\n");
	Send_String("Joy			goes to joy menu\n");
	Send_String("LPUART			goes to lpuart menu\n");
}

void printLEDHelp(){
	Send_String("Possible commands are:\n");
	Send_String("Set <id>		Turn on LED (id: 0-7 or R,G,B)\n");
	Send_String("Clear <id>		Turn off LED (id: 0-7 or R,G,B)\n");
	Send_String("Blink <id>		Blink LED five times (id: 0-7 or R,G,B)\n");
	Send_String("Status <id>		Display on/off status of LED (id: 0-7 or R,G,B)\n");
	Send_String("Toggle <id>		Invert LED state (id: 0-7 or R,G,B)\n");
}

void print7LEDHelp(){
	Send_String("Possible commands are:\n");
	Send_String("Display <val>		Display number from 0 to 9999 on the 7-LED display\n");
	Send_String("Read			Display on the terminal the number from 7-LED display\n");
}

void printJoyHelp(){
	Send_String("Possible commands are:\n");
	Send_String("Read <id>		Display on the terminal the current state of joystick buttons\n");
	Send_String("			(id: L – left, R – right, U – up, D – down, C - center)\n");
}

void printLPUARTHelp(){
	Send_String("Possible commands are:\n");
	Send_String("Status			Display baudrate, number of databits and parity bits\n");
}

void getHelp(char* command, PossibleLevels* currentLevel){
	PossibleLevels curTemp = *currentLevel;
	switch (curTemp) {
		case INIT:
			printMainMenuHelp();
			strcpy(command, "");
			break;
		case LED_DIS:
			print7LEDHelp();
			strcpy(command, "");
			break;
		case LED:
			printLEDHelp();
			strcpy(command, "");
			break;
		case LPUART_LVL:
			printLPUARTHelp();
			strcpy(command, "");
			break;
		case JOY:
			printJoyHelp();
			strcpy(command, "");
			break;
	}
}

int menuNavigation(char* command, PossibleLevels* currentLevel){
	if(strcasecmp(command, "JOY") == 0){
		*currentLevel = JOY;
		strcpy(command, "");
		return 1;
	}

	if(strcasecmp(command, "LPUART") == 0){
		*currentLevel = LPUART_LVL;
		strcpy(command, "");
		return 1;
	}

	if(strcasecmp(command, "LED") == 0){
		*currentLevel = LED;
		strcpy(command, "");
		return 1;
	}


	if(strcasecmp(command, "7LED") == 0){
		*currentLevel = LED_DIS;
		strcpy(command, "");
		return 1;
	}

	if(strcasecmp(command, "UP") == 0){
		*currentLevel = INIT;
		strcpy(command, "");
		return 1;
	}

	return 0;
}

int handleLedCommands(char* firstWord, char* secondWord, char** currentlyBlinkingLed){

	if(strcasecmp(firstWord, "SET") == 0){
		if(isdigit((unsigned char)secondWord[0])){
			int ledNumber = atoi(secondWord);
			setLed(ledNumber);
		} else {
			setRGBLed(secondWord);
		}
		return 1;
	}

	if(strcasecmp(firstWord, "CLEAR") == 0){
		if(isdigit((unsigned char)secondWord[0])){
			int ledNumber = atoi(secondWord);
			resetLed(ledNumber);
		} else {
			resetRGBLed(secondWord);
		}
		return 1;
	}

	if(strcasecmp(firstWord, "TOGGLE") == 0){
		if(isdigit((unsigned char)secondWord[0])){
			int ledNumber = atoi(secondWord);
			toggleLed(ledNumber);
		} else {
			toggleRGBLed(secondWord);
		}
		return 1;
	}

	if(strcasecmp(firstWord, "STATUS") == 0){
		if(isdigit((unsigned char)secondWord[0])){
			int ledNumber = atoi(secondWord);
			if(getLedStatus(ledNumber)){
				Send_String("\nLed is on");
			} else {
				Send_String("\nLed is off");
			}
		} else {
			if(statusRGBLed(secondWord)){
				Send_String("\nLed is on");
			} else {
				Send_String("\nLed is off");
			}
		}
		return 1;
	}

	if(strcasecmp(firstWord, "BLINK") == 0){
		if(strcasecmp(*currentlyBlinkingLed, "N") == 0){
			changeCurrentlyBlinkLED(secondWord, currentlyBlinkingLed);
		} else {
			Send_String("One led is blinking already");
		}
	}

	return 0;
}

int handleJoyCommands(char* firstWord, char* secondWord){
	if(statusJoy(secondWord) == 1){
		Send_String("Joy is pressed");
		return 1;
	} else {
		Send_String("Joy is not pressed");
		return 1;
	}
	return 0;
}

int handleLPUARTCommands(char* firstWord, char* secondWord){
	char strBaudrate[MAX_BAUDRATE_LENGTH] = {0};
	int baudrate = 4000000 * 256 / LPUART -> BRR;
	sprintf(strBaudrate, "%d", baudrate);

	char strDatabits[MAX_DATABIT_INFO] = {0};
	int dataBitsValue = ((LPUART -> CR1 >> (M1_BIT_NR-1) & 2U)  | (LPUART -> CR1 >> M1_BIT_NR & 1U));

	switch(dataBitsValue){
		case 0:
			sprintf(strDatabits, "%d", 8);
			break;
		case 1:
			sprintf(strDatabits, "%d", 9);
			break;
		case 2:
			sprintf(strDatabits, "%d", 7);
			break;
	}

	char strParity[MAX_PARITY_INFO] = {0};
	if((LPUART -> CR1 & PCE) != 0){
		strcpy(strParity,"parity on");
	} else {
		strcpy(strParity,"parity off");
	}

	Send_String("\nBaudrate:		");
	Send_String(strBaudrate);
	Send_String("\nData bits:		");
	Send_String(strDatabits);
	Send_String("\nParity bits:		");
	Send_String(strParity);

	return 1;
}

int handle7LEDCommands(char* firstWord, char* secondWord, int* displayedNumber){
	if(strcasecmp(firstWord, "DISPLAY") == 0){
		int value = atoi(secondWord);
		*displayedNumber = value;
		return 1;
	}

	if(strcasecmp(firstWord, "READ") == 0){
		char strDisNumb[5] = {0};
		sprintf(strDisNumb, "%d", *displayedNumber);
		Send_String(strDisNumb);
		return 1;
	}

	return 0;
}

int isCorrectLEDCommand(char* firstWord, char* secondWord) {
    if (strcasecmp(firstWord, "Set") != 0 &&
        strcasecmp(firstWord, "Clear") != 0 &&
        strcasecmp(firstWord, "Blink") != 0 &&
        strcasecmp(firstWord, "Status") != 0 &&
        strcasecmp(firstWord, "Toggle") != 0) {
        return 0;
    }

    if (isdigit((unsigned char)secondWord[0])) {
        int ledNumber = atoi(secondWord);
        if (ledNumber >= 0 && ledNumber <= 7) {
            return 1;
        }
    } else if (strcasecmp(secondWord, "R") == 0 ||
               strcasecmp(secondWord, "G") == 0 ||
               strcasecmp(secondWord, "B") == 0) {
        return 1;
    }

    return 0;
}

int isCorrectJoyCommand(char* firstWord, char* secondWord) {
    if (strcasecmp(firstWord, "READ") != 0) {
        return 0;
    }

    if (strcasecmp(secondWord, "L") == 0 ||
        strcasecmp(secondWord, "R") == 0 ||
        strcasecmp(secondWord, "U") == 0 ||
        strcasecmp(secondWord, "D") == 0 ||
        strcasecmp(secondWord, "C") == 0) {
        return 1;
    }

    return 0;
}

int isCorrectLPUARTCommand(char* firstWord, char* secondWord){
	if(strcasecmp(firstWord, "STATUS") == 0){
		return 1;
	}

	return 0;
}

int isCorrect7LEDCommand(char* firstWord, char* secondWord){
	if (strcasecmp(firstWord, "Display") == 0) {
		int value = atoi(secondWord);
		if (value >= 0 && value <= 9999) {
			return 1;
		}
	} else if (strcasecmp(firstWord, "Read") == 0) {
		return 1;
	}

	return 0;
}

void changeCurrentlyBlinkLED(char* secondWord, char** currentlyBlinkingLed) {
	static char ledStr[2];
	if (isdigit((unsigned char)secondWord[0])) {
        int ledNumber = atoi(secondWord);
        if (ledNumber >= 0 && ledNumber <= 7) {
            snprintf(ledStr, sizeof(ledStr), "%d", ledNumber);
            *currentlyBlinkingLed = ledStr;
        }
    } else if (strcasecmp(secondWord, "R") == 0 ||
               strcasecmp(secondWord, "G") == 0 ||
               strcasecmp(secondWord, "B") == 0) {
    	ledStr[0] = secondWord[0];  // Copy 'R', 'G', or 'B'
		ledStr[1] = '\0';
		*currentlyBlinkingLed = ledStr;
    }
}
