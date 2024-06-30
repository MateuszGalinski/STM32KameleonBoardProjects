# STM32 Kameleon Board Projects

Welcome to the STM32 Kameleon Board Projects repository. This collection showcases various projects developed for the STM32 Kameleon Board. Only main.c and my own libraries are provided so in order to run them you have to generate your own stm ide project and insert them there.

# 1. **7 LED Display Counter (No HAL)**: A project demonstrating a 7-segment LED display counter without using the Hardware Abstraction Layer (HAL).

## Overview
  This project showcases the use of an STM32 microcontroller to interface with a seven-segment display and a joystick. The main functionality includes displaying a 4-digit number on the seven-segment display and incrementing or decrementing this number based on joystick input. This project involves configuring GPIOs, setting up the system clock, and initializing peripherals.

## Features
  Seven-Segment Display Control: Displays a 4-digit number.
  Joystick Input Handling: Reads joystick input to modify the displayed number.
  GPIO Configuration: Proper configuration of GPIO pins for both output (display) and input (joystick).
  System Clock Setup: Configures the system clock for accurate timing operations.

## Code Structure
  ### Main Files
      main.c: Contains the core logic, including initialization and the main loop.
  ### Key Functions
      SystemClock_Config(): Configures the system clock.
      Display_Init(): Initializes the GPIOs for the seven-segment display.
      JoyStick_Init(): Initializes the GPIOs for joystick input.
      Check_Input(): Reads the joystick input to determine whether to increment or decrement the display number.
      Diodes_Init(): Initializes additional LEDs (optional for debugging or indicators).
      Set_Moder(volatile uint32_t* portModer, uint32_t pins, uint32_t mode): Configures the mode (input/output) of GPIO pins.

# 2. **Blinking LED with Joystick Control**: A simple LED blinking project controlled via a joystick.
## Overview
  This project demonstrates the use of an STM32 microcontroller to control a series of LEDs, making them blink in a predefined sequence. The LEDs will blink from left to right and then from right to left. The project includes GPIO configuration, system clock setup, and control of LED states using the HAL library.

## Features
  LED Blinking Sequence: LEDs blink from left to right and right to left.
  GPIO Configuration: Proper configuration of GPIO pins for output.
  System Clock Setup: Configures the system clock for accurate timing operations.

## Code Structure
  ### Main Files
    main.c: Contains the core logic, including initialization and the main loop.
  ### Key Functions
    SystemClock_Config(): Configures the system clock.
    MX_GPIO_Init(): Initializes the GPIO pins.
    Blink_Leds_To_Left(): Blinks the LEDs from left to right.
    Blink_Leds_To_Right(): Blinks the LEDs from right to left.
    Get_Led(int ledNumber): Returns the GPIO pin corresponding to the LED number.
    
# 3. **Counter**: A basic counter application.

## Code Explanation
The main components of the code are:

1. **Initialization:**
    - `HAL_Init()`: Initializes the HAL library.
    - `SystemClock_Config()`: Configures the system clock.
    - `MX_GPIO_Init()`: Initializes the GPIO ports.
    - `JoyStick_Init()`: Initializes the joystick GPIO port.
    - `Diodes_Init()`: Initializes the LEDs GPIO ports.

2. **Infinite Loop:**
    - Displays the current digit on the 7-segment display.
    - Checks the joystick input to increment or decrement the digit.
    - Updates the displayed digit accordingly.

### Key Definitions
- `COUNTER_TIME`: Time interval for counting.
- `JOY_OK`: Pin definition for joystick OK button.
- `SEG_A, SEG_B, ..., SEG_G`: Definitions for 7-segment display segments.
- `DIG1, DIG2, ..., DIG4`: Definitions for digit control pins.
- `SEG_GPIO, DIGPORT`: Definitions for segment and digit GPIO ports.
- `LED0, LED1, ..., LED7`: Definitions for LED pins.
- `ALL_SEGMENTS`: Combination of all segment pins.

### Functions
- `MX_GPIO_Init()`: Initializes the GPIO ports.
- `JoyStick_Init()`: Initializes the joystick.
- `Check_Input()`: Checks joystick input.
- `Diodes_Init()`: Initializes the LEDs.
- `SystemClock_Config()`: Configures the system clock.

# 4. **Counter No HAL with Joystick Control**: A counter project controlled by a joystick without using HAL.

## Code Explanation
The main components of the code are:

### Initialization
- `HAL_Init()`: Initializes the HAL library.
- `SystemClock_Config()`: Configures the system clock.
- `My_Clock_Init()`: Configures the custom clock settings.
- `Display_Init()`: Initializes the 7-segment display.
- `JoyStick_Init()`: Initializes the joystick GPIO port.
- `Diodes_Init()`: Initializes the LEDs GPIO ports.

### Infinite Loop
- Displays the current digit on the 7-segment display.
- Checks the joystick input to increment or decrement the digit.
- Updates the displayed digit accordingly.

### Key Definitions
- `COUNTER_TIME`: Time interval for counting.
- `JOY_OK`: Pin definition for joystick OK button.
- `SEG_A, SEG_B, ..., SEG_G`: Definitions for 7-segment display segments.
- `DIG(x)`: Macro for digit control pins.
- `LED0, LED1, ..., LED7`: Definitions for LED pins.
- `ALL_SEGMENTS`: Combination of all segment pins.

### Functions
- `MX_GPIO_Init()`: Initializes the GPIO ports.
- `My_Clock_Init()`: Custom clock initialization.
- `My_Delay(int milliseconds)`: Custom delay function using TIM6.
- `Display_Init()`: Initializes the 7-segment display.
- `JoyStick_Init()`: Initializes the joystick.
- `Check_Input()`: Checks joystick input.
- `Diodes_Init()`: Initializes the LEDs.
- `Set_Moder(GPIO_My port, uint32_t pins, uint32_t mode)`: Sets the mode for GPIO pins.

### Segment Data
The `segments` array contains the segment combinations for displaying digits 0-9.

# 5. **Interrupt-Based UART Receive (No HAL)**: Implementing UART communication using interrupts, without HAL.

## Project Overview

This STM32 firmware project implements a simple system that utilizes an LPUART (Low Power UART) interface to receive single-digit numbers from a serial terminal. The received numbers are displayed on a 7-segment display, and the system handles various conditions like input validation and FIFO buffer management. It also uses a basic timer to periodically update the display based on the received data.

## Features

- **LPUART Communication**: Receives single-digit numbers from a serial terminal.
- **7-Segment Display**: Displays the received number on a 7-segment display.
- **FIFO Buffer**: Manages incoming data and handles overflow conditions.
- **Error Handling**: Validates received data and sends feedback characters.
- **LED Indicators**: Displays error states using onboard LEDs.

Helper Functions
Display(int number, int failStatus): Updates the 7-segment display based on the received number.

SystemClock_Config(void): Configures the system clock settings.

## Function Descriptions
void LPUART1_IRQHandler(void): Handles LPUART interrupts to read incoming data and manage the FIFO buffer.
void SystemClock_Config(void): Configures the system clock and initializes clock sources.
void Display(int number, int failStatus): Manages the 7-segment display to show the received number or handle failure conditions.
void Error_Handler(void): Enters an infinite loop if an error occurs.
## Code Dependencies
myDefines.h: Contains definitions for GPIO pins, segment configurations, and other constants.
usefulfunctions.h: Provides utility functions used throughout the program.
lpuart.h: Defines functions and configurations for LPUART communication.
fifo.h: Implements FIFO buffer operations for managing incoming serial data.
## Known Issues
Ensure proper error handling and debugging when the FIFO buffer overflows.
The program expects valid single-digit numbers; other inputs may cause errors.
There are polling send functions in handler which should be substituted for interrupt based sending system simillar to receive one

# 6. **Polling UART (No HAL)**: UART communication using polling method, without HAL.

## Project Overview

This STM32 firmware project is designed to demonstrate basic UART (Universal Asynchronous Receiver/Transmitter) communication and character case conversion. The program initializes the LPUART (Low Power UART) interface, sends a sequence of characters, and performs real-time character case conversion based on received input.

## Features

- **Character Transmission**: Sends the alphabet in both uppercase and lowercase over UART.
- **Greeting Message**: Sends a "Hello world" message over UART.
- **Echo and Case Conversion**: Receives characters from UART, converts their case, and sends the converted characters back.
- **UART Communication**: Uses LPUART to communicate with a serial terminal.

## Helper Functions
void LPUART_Init(void): Initializes the LPUART peripheral for communication.
void Diodes_Init(void): Initializes GPIO pins for LEDs.
void Send_Char(char c): Sends a single character over UART.
void Send_String(const char *str): Sends a string of characters over UART.
char Recv_char(void): Receives a single character from UART.
Known Issues
Case Conversion: The program currently only performs case conversion on alphabetic characters.

# 7. **UART-Based Text Menu for Dev Board Control**: A text menu interface for controlling the development board via UART.

## Overview
  This project implements a simple command-line interface for embedded systems using the LPUART (Low Power Universal Asynchronous Receiver Transmitter) module on a microcontroller. The system includes a FIFO (First In, First Out) buffer for UART communication, a menu system for various commands, and utility functions for hardware manipulation.

## Key Components
  FIFO Buffer (fifo.c and fifo.h)
  
  Implements a circular buffer for UART data reception.
  LPUART Communication (lpuart.c and lpuart.h)
  
  Initializes the LPUART peripheral and provides functions for sending and receiving characters and strings.
  Menu Display and Command Handling (menuDisplay.c and menuDisplay.h)
  
  Manages the command-line interface, processes commands, and provides help information.
  Useful Functions (usefulfunctions.c and usefulfunctions.h)
  
  Contains utility functions for hardware control and data manipulation.
  Main Application (main.c)
  
  Configures hardware, initializes components, and runs the main application loop.

## Functions
### FIFO Buffer
- FIFO_Init(): Initializes the FIFO buffer.
- FIFO_Empty(): Empties the FIFO buffer.
- FIFO_IsEmpty(): Checks if the FIFO buffer is empty.
- FIFO_IsFull(): Checks if the FIFO buffer is full.
- FIFO_Put(): Adds data to the FIFO buffer.
- FIFO_Get(): Retrieves data from the FIFO buffer.
### LPUART Communication
- LPUART_Init(): Initializes the LPUART peripheral.
- Send_Char(): Sends a single character over LPUART.
- Send_String(): Sends a string over LPUART.
- Recv_Char(): Receives a single character from LPUART.
### Menu Display and Command Handling
- commandHandler(): Processes commands from the user.
- extractWords(): Extracts the first and second words from a command.
- printWelcomeMessage(): Prints the welcome message.
- printMainMenuHelp(): Displays help for the main menu.
- printLEDHelp(): Displays help for LED commands.
- print7LEDHelp(): Displays help for 7-LED display commands.
- printJoyHelp(): Displays help for joystick commands.
- printLPUARTHelp(): Displays help for LPUART commands.
- getHelp(): Provides command help based on the current menu level.
- menuNavigation(): Handles navigation between menus.
- handleLedCommands(): Processes LED-related commands.
- handleJoyCommands(): Processes joystick commands.
- handleLPUARTCommands(): Processes LPUART commands.
- handle7LEDCommands(): Processes 7-LED display commands.
### Useful Functions
- My_Clock_Init(): Initializes the system clock.
- My_Delay(): Delays for a specified amount of time.
- Display_Init(): Initializes the 7-LED display.
- JoyStick_Init(): Initializes the joystick.
- Check_Input(): Checks if the joystick button is pressed.
- Diodes_Init(): Initializes the LEDs.
- Set_Moder(): Configures the GPIO mode.
- displayNumber(): Displays a number on the 7-LED display.
- setRGBLed(): Sets the state of the RGB LED.
- resetRGBLed(): Resets the RGB LED.
- toggleRGBLed(): Toggles the RGB LED state.
- statusRGBLed(): Checks the status of the RGB LED.
- setLed(): Sets a specific LED.
- resetLed(): Resets a specific LED.
- toggleLed(): Toggles a specific LED.
- getLedStatus(): Checks the status of a specific LED.
- statusJoy(): Checks the status of the joystick.
- blinkLedInMain(): Manages LED blinking in the main loop.
