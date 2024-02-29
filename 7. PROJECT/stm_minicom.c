#include "stm32f4xx_hal.h"
#include <stdio.h> // For sprintf function
#include <string.h>

void Error_Handler(void)
{
    while (1)
    {
        // Add error handling or debugging code here
    }
}

// Define GPIO pins for the common cathode dual-color LED module
#define YELLOW_LED_PIN GPIO_PIN_0

// User switch connected to PC13
#define USER_SWITCH_PIN GPIO_PIN_13

// UART handle
UART_HandleTypeDef huart2;

void GPIO_Init(void);
void UART_Init(void);

int main(void)
{
    HAL_Init();

    // Initialize HAL Library
    HAL_InitTick(0);  // Initialize the HAL Tick (if required)

    // Initialize peripherals
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_Init();
    UART_Init(); // Initialize UART communication

    char message[100]; // Buffer for the message
    uint8_t switchValue = 0; // Variable to track switch state
    uint8_t previousSwitchState = GPIO_PIN_SET; // Initialize with a different state

    while (1)
    {
        // Read the state of the user switch
        uint8_t currentSwitchState = HAL_GPIO_ReadPin(GPIOC, USER_SWITCH_PIN);

        if (currentSwitchState != previousSwitchState)
        {
            previousSwitchState = currentSwitchState;

            if (currentSwitchState == GPIO_PIN_RESET)
            {
                // User switch is pressed
                HAL_GPIO_WritePin(GPIOA, YELLOW_LED_PIN, GPIO_PIN_SET);
                switchValue = 1;
              //  sprintf(message, "LED is off\r\n");
            }
            else
            {
                // User switch is not pressed
                HAL_GPIO_WritePin(GPIOA, YELLOW_LED_PIN, GPIO_PIN_RESET);
                switchValue = 0;
            }

            // Send the switch value to Minicom
            sprintf(message, "Switch value = %d\r\n", switchValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
        }
    }
}

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = YELLOW_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure the user switch pin as input with pull-up
    GPIO_InitStruct.Pin = USER_SWITCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200; // Adjust the baud rate as needed
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}
