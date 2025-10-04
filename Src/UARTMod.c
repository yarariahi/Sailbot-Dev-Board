#include "UARTMod.h"
#include "main.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

static uint8_t rx1_data;
static uint8_t rx2_data;



void UARTMod_Init(void)
{
    // Start listening for both UARTs
    HAL_UART_Receive_IT(&huart1, &rx1_data, 1);
    HAL_UART_Receive_IT(&huart2, &rx2_data, 1);
}

void UARTMod_RxCpltCallback(UART_HandleTypeDef *huart)
{
    const uint32_t TX_TIMEOUT_MS = 5;

    if (huart->Instance == USART1) {
        // Byte arrived from PC/PuTTY -> forward to link
        (void)HAL_UART_Transmit(&huart2, &rx1_data, 1, TX_TIMEOUT_MS);
        // Re-arm reception on PC side
        HAL_UART_Receive_IT(&huart1, &rx1_data, 1);
    }
    else if (huart->Instance == USART2) {
        // Byte arrived from LINK (other STM32) -> forward to PC
        (void)HAL_UART_Transmit(&huart1, &rx2_data, 1, TX_TIMEOUT_MS);
        // Re-arm reception on LINK side
        HAL_UART_Receive_IT(&huart2, &rx2_data, 1);
    }
}

void UARTMod_ErrorHandle(UART_HandleTypeDef *huart)
{
	  if (huart->Instance == USART1) {
	    HAL_UART_Receive_IT(&huart1, &rx1_data, 1);
	  } else if (huart->Instance == USART2) {
	    HAL_UART_Receive_IT(&huart2, &rx2_data, 1);
	  }
}
