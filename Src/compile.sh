#!/bin/bash 
arm-none-eabi-gcc main.c \
  stm32f0xx_it.c \
  system_stm32f0xx.c \
  startup_stm32f031x6.s \
  -fno-common -ffunction-sections -fdata-sections \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
  ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
  -I ../Inc \
  -I ../../../../../../Drivers/CMSIS/Core/Include/ \
  -I ../../../../../../Drivers/STM32F0xx_HAL_Driver/Inc/ \
  -I ../../../../../../Drivers/CMSIS/Device/ST/STM32F0xx/Include/ \
  -I ../../../../../../Drivers/BSP/STM32F0xx_Nucleo_32/ \
  -T ../SW4STM32/STM32F031K6_NUCLEO/STM32F031K6Tx_FLASH.ld \
  -specs=rdimon.specs -mfloat-abi=soft -mcpu=cortex-m0 -mthumb \
  -Os -o sus.bin -Wl,--gc-sections \

  openocd -f ../flash/openocd.cfg -c "program ../Src/sus.bin verify reset exit"
  # ../../../../../../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
