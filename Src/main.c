#include "main.h"
#include "Legacy/stm32_hal_legacy.h"
#include "stm32f031x6.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h" 
#include "stm32f0xx_hal_dma.h" 
#include "stm32f0xx_hal_msp.c"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_uart.h"
#include "stm32f0xx_hal_usart.h"
#include "stm32f0xx_nucleo_32.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define FAN_COUNT 1
#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
#define CHUNK_SIZE 2

void initUart(UART_HandleTypeDef* uart);
void uartPins();
void fuck(int);


uint8_t errorMessage[] = "\n\rError Found\n\r";
uint8_t bufferCOBS[64] = {0}; // This is stupid, malloc should be done instead
uint8_t newline[] = "\n\r"; 

UART_HandleTypeDef thing = {0}; 

uint8_t Cflag = 0x7E;
uint8_t Ccode = 0x7D;

struct fan {
  uint8_t fanId; // holds 16 values
  uint8_t speed; // holds 256 for speed
}packed;

struct driverData {
  struct fan full[FAN_COUNT];
};

size_t cobs(void* stuff, uint8_t* buffer, size_t s) {

  int dist = 1;
  uint8_t* srcPos = (uint8_t*)stuff;
  uint8_t* writePos = buffer + 1; 

  // Reserve first byte of our buffer for the first 0

  for (;s--;) {
    // byte not 0, write it;
    if(*srcPos) {
    *writePos = *srcPos;
    }

    if(!*srcPos || dist == 0xFF) {
    *(writePos - dist) = dist; // Grab current position, go back the distance since last 0 and write the distance there 
    dist = 0;
    }
    
    // It is another day...
    dist++;
    writePos++;
    srcPos++;
  }
    *(writePos - dist) = dist; // do it once more
    *++writePos = 0;

    return (writePos - buffer); // Length of encoded data
}

// Cobs Decode data
size_t unCobs (void* stuff, uint8_t* buffer, size_t s) {
  uint8_t* writePos = buffer - 1;
  uint8_t* srcPos = stuff;
  int dist = {0};

  for (;*srcPos;) {
    dist = *srcPos;
    *writePos = 0;

    // Increase SrcPos by dist and go again
    for ( int a = 0; a < dist; a++) {
      srcPos++;
      writePos++;
      *writePos = *srcPos;
    }
  }
  return (writePos - buffer);
};

// Incomplete. Receives on the serial line until there is a start of the packet, then starts reading the whole packet until the end. This is for COBS. To be used later
void getPacket(UART_HandleTypeDef* art, uint8_t* buffer) {
  uint8_t byte[10] = {0};
  do {
      HAL_UART_Receive(art, buffer, 1, 1000);
  } while (*buffer != 3);
  fuck(1000);
}


// Stop fails in a visible way. Blinks the LED to signal demise.
void fuck(int sec) {
  LED3_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef led;
  led.Mode  = GPIO_MODE_OUTPUT_PP;
  led.Pull  = GPIO_MODE_OUTPUT_PP;
  led.Speed = GPIO_SPEED_MEDIUM;
  led.Pin = LED3_PIN;
  HAL_GPIO_Init(LED3_GPIO_PORT, &led);
  while (1)
  {
    HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
    HAL_Delay(sec);
  }
};

// send in chunks of max 16 bytes until the end of the string. Used for text only and a in house replacement for printLn
void printLn(UART_HandleTypeDef* uart, char* message){
  int length = strlen(message);
  char* p = message;
  for (size_t c = 0; c < length;){
    if ( (length - c) >= CHUNK_SIZE ) {
      HAL_UART_Transmit(uart, (message + c) , CHUNK_SIZE, 100);
      c += CHUNK_SIZE;
    } else {
      HAL_UART_Transmit(uart, (message + c) , length - c, 100);
      c += length - c;
    }
  }
};
  

void initUart(UART_HandleTypeDef* uart) {
  uart->Instance = USART1;
  uart->Init.BaudRate = 9600;
  uart->Init.WordLength = UART_WORDLENGTH_8B;
  uart->Init.StopBits = UART_STOPBITS_1;
  uart->Init.Parity = UART_PARITY_NONE;
  uart->Init.Mode = UART_MODE_TX_RX;
  uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  if (HAL_UART_Init(uart) != HAL_OK)
    fuck(5000);


};


void uartPins() {
    GPIO_InitTypeDef sus = {0};
    sus.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    sus.Mode = GPIO_MODE_AF_PP;
    sus.Speed = GPIO_SPEED_FREQ_HIGH;
    sus.Alternate = GPIO_AF1_USART1;
    sus.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &sus);
}

void configurePins() {
  //Initialize GPIO Resources 
    GPIO_InitTypeDef sus = {0};
    sus.Pin = GPIO_PIN_8;
    sus.Mode = GPIO_MODE_AF_PP;
    sus.Speed = GPIO_SPEED_MEDIUM;
    sus.Alternate = GPIO_AF2_TIM1;
    sus.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &sus);
}

void initializeTIMLL() {
   __HAL_RCC_TIM1_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_USART1_CLK_ENABLE();
  configurePins();
}


void uartInit (){};

// Initialize tim to PWM at 21 khz to channel provided 
void timInit (TIM_HandleTypeDef* tim) {
tim->Instance = TIM1;
tim->Init.Period = 254;
tim->Init.Prescaler = 8;
tim->Init.CounterMode = TIM_COUNTERMODE_UP;
tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
if (HAL_TIM_PWM_Init(tim) != HAL_OK)
  fuck(5000);

TIM_OC_InitTypeDef oc = {0};
oc.OCMode = TIM_OCMODE_PWM1;
oc.Pulse = 244;
oc.OCPolarity = TIM_OCPOLARITY_HIGH;
oc.OCFastMode = TIM_OCFAST_ENABLE;

if (HAL_TIM_PWM_ConfigChannel(tim, &oc, TIM_CHANNEL_1) != HAL_OK)
  fuck(5000);
};




// Set up Clocks
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* No HSE Oscillator on Nucleo, Activate PLL with HSI/2 as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  //TIM1->CCR1 = bufferCOBS[0];
  printLn(&thing, "Received callback");
  HAL_UART_Receive_IT(huart, &bufferCOBS[0], 1);
}


int main () {
  int32_t CH1_DC = 0;
  uint8_t xbuffer[64];
  char str[4];
  HAL_Init();
  SystemClock_Config();
  initializeTIMLL();
  // Init Done here
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

  TIM_HandleTypeDef tim = {0};
  timInit(&tim);

  uartPins();
  initUart(&thing);
  __HAL_UART_ENABLE(&thing);

  HAL_TIM_PWM_Start(&tim, TIM_CHANNEL_1);
  HAL_UART_Receive_IT(&thing, bufferCOBS, 1);
  while(1) { 
    //HAL_Delay(100);
    //TIM1->CCR1 = 0;
    //TIM1->CCR1 = 255;
    fuck(100);
  }

}
