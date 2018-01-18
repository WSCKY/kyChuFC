#include "DMAIrqManage.h"

void DMA1_Stream0_IRQHandler(void)
{
#if (UART8_ENABLE)
#if (UART8_TX_DMA_ENABLE)
	UART8_DMA_TX_IRQHandler();
#endif /* UART8_TX_DMA_ENABLE */
#endif /* UART8_ENABLE */
}

void DMA1_Stream1_IRQHandler(void)
{
#if (USART3_ENABLE)
#if (USART3_RX_DMA_ENABLE)
	USART3_DMA_RX_IRQHandler();
#endif /* USART3_RX_DMA_ENABLE */
#endif /* USART3_ENABLE */
#if (UART7_ENABLE)
#if (UART7_TX_DMA_ENABLE)
	UART7_DMA_TX_IRQHandler();
#endif /* UART7_TX_DMA_ENABLE */
#endif /* UART7_ENABLE */
}

void DMA1_Stream3_IRQHandler(void)
{
#if (SPI2_ENABLE)
#if (SPI2_RX_DMA_ENABLE)
  SPI2_DMA_RX_IRQHandler();
#endif /* SPI2_RX_DMA_ENABLE */
#endif /* SPI2_ENABLE */
#if (UART7_ENABLE)
#if (UART7_RX_DMA_ENABLE)
	UART7_DMA_RX_IRQHandler();
#endif /* UART7_RX_DMA_ENABLE */
#endif /* UART7_ENABLE */
}

void DMA1_Stream4_IRQHandler(void)
{
#if (SPI2_ENABLE)
#if (SPI2_TX_DMA_ENABLE)
  SPI2_DMA_TX_IRQHandler();
#endif /* SPI2_TX_DMA_ENABLE */
#endif /* SPI2_ENABLE */
#if (USART3_ENABLE)
#if (USART3_TX_DMA_ENABLE)
	USART3_DMA_TX_IRQHandler();
#endif /* USART3_TX_DMA_ENABLE */
#endif /* USART3_ENABLE */
}

void DMA1_Stream6_IRQHandler(void)
{
#if (UART8_ENABLE)
#if (UART8_RX_DMA_ENABLE)
	UART8_DMA_RX_IRQHandler();
#endif /* UART8_RX_DMA_ENABLE */
#endif /* UART8_ENABLE */
}

void DMA2_Stream0_IRQHandler(void)
{
#if (SPI4_ENABLE)
#if (SPI4_RX_DMA_ENABLE)
  SPI4_DMA_RX_IRQHandler();
#endif /* SPI4_RX_DMA_ENABLE */
#endif /* SPI4_ENABLE */
}

void DMA2_Stream1_IRQHandler(void)
{
#if (SPI4_ENABLE)
#if (SPI4_TX_DMA_ENABLE)
  SPI4_DMA_TX_IRQHandler();
#endif /* SPI4_TX_DMA_ENABLE */
#endif /* SPI4_ENABLE */
}

void DMA2_Stream4_IRQHandler(void)
{
#if (ADC1_ENABLE)
#if (ADC1_DMA_IRQ_ENABLE)
	ADC1_DMA_IRQHandler();
#endif /* ADC1_DMA_IRQ_ENABLE */
#endif /* ADC1_ENABLE */
}
