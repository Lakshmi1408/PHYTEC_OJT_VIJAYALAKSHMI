//#include "stm32f4xx_hal.h"
#include"main.h"

#include "stdio.h" // For sprintf function
#include "string.h"
 uint8_t switchValue = 0;

void mqtt_data_send(uint8_t n);
void WE10_Init ();
void MQTT_Init();
void Error_Handler(void);


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
    WE10_Init ();
    MQTT_Init();
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
            mqtt_data_send(0);
HAL_Delay(100);
        }
        else
        {
            // User switch is pressed
            HAL_GPIO_WritePin(GPIOA, YELLOW_LED_PIN, GPIO_PIN_SET);
            switchValue = 1;
            // Send a message to Minicom
            sprintf(message, "LED is on:%d\r\n",switchValue);
            HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
            mqtt_data_send(1);
            HAL_Delay(100);
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
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */

}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 38400; // Adjust the baud rate as needed
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
void WE10_Init ()
{
	char buffer[128];
	/* CMD+RESET **/
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+RESET\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);

	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);


	/*  CMD+WIFIMODE=1  **/
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+WIFIMODE=1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);

	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);


	/* CMD+CONTOAP=SSID,PASSWD **/
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0],"CMD+CONTOAP=Realme7pro,kundeti111\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);
	//memset(&buffer[0],0x00,strlen(buffer));
	HAL_Delay(2000);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);

	/* CMD?WIFI**/
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD?WIFI\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);
//	memset(&buffer[0],0x00,strlen(buffer));
//	HAL_Delay(500);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);


}
void MQTT_Init()
{

	char buffer[128];

	/*CMD+MQTTNETCFG */
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+MQTTNETCFG=dev.rightech.io,1883\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);
	//memset(&buffer[0],0x00,strlen(buffer));
	//HAL_Delay(500);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 10000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 10000);


	/*CMD+MQTTCONCFG---->LED */
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+MQTTCONCFG=3,mqtt-vijayalakshmivijji1408-bafbxc,,,,,,,,,\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);
	//memset(&buffer[0],0x00,strlen(buffer));
	//HAL_Delay(500);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);


	/*CMD+MQTTSTART */
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+MQTTSTART=1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);
//	memset(&buffer[0],0x00,strlen(buffer));
	HAL_Delay(5000);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);

	/*CMD+MQTTSUB **/
	//memset(&buffer[0],0x00,strlen(buffer));
	sprintf (&buffer[0], "CMD+MQTTSUB=base/relay/led1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_Delay(500);
	HAL_UART_Receive(&huart1, (uint8_t*)buffer, strlen(buffer), 1000);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);

}

void mqtt_data_send(uint8_t n)
{
char buffer[50];

sprintf(&buffer[0], "CMD+MQTTPUB=reading/mini-colour,%d\r\n", n);
HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
HAL_Delay(100);
}
void Error_Handler(void)
{
    while (1)
    {
        // Add error handling or debugging code here
    }
}
