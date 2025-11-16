#ifndef UART_RING_H
#define UART_RING_H

#include "main.h"   // for UART_HandleTypeDef
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void UartRing_Init(UART_HandleTypeDef *huart);
void UartRing_Start(void);
int  UartRing_GetByte(uint8_t *byte);      // returns 1 if a byte is read, 0 if buffer empty
void UartRing_OnRxCompleteFromISR(void);   // to be called from HAL UART Rx callback

#ifdef __cplusplus
}
#endif

#endif // UART_RING_H
