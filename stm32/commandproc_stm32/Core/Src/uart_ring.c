#include "uart_ring.h"

#define UART_RING_BUFFER_SIZE 128

static UART_HandleTypeDef *s_huart      = NULL;
static uint8_t             s_rxByte     = 0;
static uint8_t             s_buffer[UART_RING_BUFFER_SIZE];
static volatile uint16_t   s_head       = 0;
static volatile uint16_t   s_tail       = 0;

static void UartRing_PushFromISR(uint8_t byte)
{
    uint16_t nextHead = (uint16_t)((s_head + 1u) % UART_RING_BUFFER_SIZE);

    if (nextHead != s_tail)
    {
        s_buffer[s_head] = byte;
        s_head = nextHead;
    }
    else
    {
        // Buffer overflow, drop byte (or handle error)
    }
}

void UartRing_Init(UART_HandleTypeDef *huart)
{
    s_huart  = huart;
    s_head   = 0u;
    s_tail   = 0u;
}

void UartRing_Start(void)
{
    if (s_huart != NULL)
    {
        (void)HAL_UART_Receive_IT(s_huart, &s_rxByte, 1u);
    }
}

void UartRing_OnRxCompleteFromISR(void)
{
    UartRing_PushFromISR(s_rxByte);

    if (s_huart != NULL)
    {
        (void)HAL_UART_Receive_IT(s_huart, &s_rxByte, 1u);
    }
}

int UartRing_GetByte(uint8_t *byte)
{
    if (s_head == s_tail)
    {
        return 0; // buffer empty
    }

    *byte = s_buffer[s_tail];
    s_tail = (uint16_t)((s_tail + 1u) % UART_RING_BUFFER_SIZE);
    return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == s_huart)
    {
        UartRing_OnRxCompleteFromISR();
    }
}
