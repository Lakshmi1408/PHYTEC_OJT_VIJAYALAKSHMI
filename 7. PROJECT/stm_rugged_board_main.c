#include "stm32f4xx_hal.h"
#include"main.h"

#include "stdio.h" // For sprintf function
#include "string.h"
 uint8_t switchValue = 0;

//void mqtt_data_send(uint8_t n);


void Error_Handler(void);
void uart1_init(void);


// Define GPIO pins for the common cathode dual-color LED module
#define YELLOW_LED_PIN GPIO_PIN_0

// User switch connected to PC13
#define USER_SWITCH_PIN GPIO_PIN_13

// UART handle
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);

void uart1_init(void)
{
	RCC->AHB1ENR |= 1;
	RCC->APB2ENR |= 0x00000010;
	GPIOA->MODER &= ~0x003C0000;
	GPIOA->MODER |= 0x00280000;
	GPIOA->AFR[1] &= ~0x0FF0;
	GPIOA->AFR[1] |= 0x0770 ; /* alt8 for UART4 */
	USART1->BRR = 0x0683; /* 9600 baud @ 16 MHz */
	USART1->CR1 = 0x000C;/* enable Tx, Rx, 8-bit data */
	USART1->CR2 = 0x0000; /* 1 stop bit*/
	USART1->CR3 = 0x0000; /* no flow control */
	USART1->CR1 |= 0x2000; /*enable UART4 */

}
void uart1_write(uint8_t ch) {
while (!(USART1->SR & 0x0080)) {
} // wait until Tx buffer empty
USART1->DR = (ch & 0xFF);
}
int main(void)
{
    HAL_Init();

    // Initialize HAL Library
    HAL_InitTick(0);  // Initialize the HAL Tick (if required)

    // Initialize peripherals
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART1_UART_Init();// Initialize UART communication
    uart1_init();
   // uart1_write(ch);
    char message[100]; // Buffer for the message
    while (1)
    {
        // Read the state of the user switch
        uint8_t switchValue = HAL_GPIO_ReadPin(GPIOC, USER_SWITCH_PIN);

        if (switchValue == GPIO_PIN_SET)
        {
            // User switch is not pressed
            HAL_GPIO_WritePin(GPIOA, YELLOW_LED_PIN, GPIO_PIN_RESET);

            // Send a message to Minicom
            switchValue = 0;
            sprintf(message, "LED is off:%d\r\n",switchValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
            uart1_write(switchValue);
          //  mqtt_data_send(0);
HAL_Delay(500);
        }
        else
        {
            // User switch is pressed
            HAL_GPIO_WritePin(GPIOA, YELLOW_LED_PIN, GPIO_PIN_SET);
            switchValue = 1;
            // Send a message to Minicom
            sprintf(message, "LED is on:%d\r\n",switchValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
            //mqtt_data_send(1);
            uart1_write(switchValue);
            HAL_Delay(500);
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
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    while(1)
    {

    }
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */

}

static void MX_USART2_UART_Init(void)
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
    	while(1)
    	    {

    	    }
    }
}


