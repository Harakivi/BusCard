#include "Spi.h"
#include "stm32f4xx_hal.h"

static SPI_HandleTypeDef hspi1;
static DMA_HandleTypeDef hdma_spi1_tx;
static void (*OnTransferComplete)();

bool Spi1Init(void (*onTransferComplete)())
{
    OnTransferComplete = onTransferComplete;

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;

    return HAL_SPI_Init(&hspi1) == HAL_OK;
}

bool Spi1DmaInit(void (*onTransferComplete)())
{
    OnTransferComplete = onTransferComplete;
    bool res = true;
    __HAL_RCC_DMA2_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

    hdma_spi1_tx.Instance = DMA2_Stream5;
    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    res = HAL_DMA_Init(&hdma_spi1_tx) == HAL_OK;
    __HAL_LINKDMA(&hspi1, hdmatx, hdma_spi1_tx);
    return res;
}

void Spi1Write(uint8_t *buff, uint32_t size)
{
    while (size > UINT16_MAX)
    {
        HAL_SPI_Transmit(&hspi1, buff, UINT16_MAX, HAL_MAX_DELAY);
        size -= UINT16_MAX;
    }
    HAL_SPI_Transmit(&hspi1, buff, size, HAL_MAX_DELAY);

    if (OnTransferComplete)
    {
        OnTransferComplete();
    }
}

void Spi1WriteDma(uint8_t *buff, uint32_t size)
{
    while (size > UINT16_MAX)
    {
        while (hdma_spi1_tx.State != HAL_DMA_STATE_READY)
        {
        }
        HAL_SPI_Transmit_DMA(&hspi1, buff, UINT16_MAX);
        size -= UINT16_MAX;
    }
    HAL_SPI_Transmit_DMA(&hspi1, buff, size);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
    }
}

void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi1_tx);
    if (hdma_spi1_tx.State == HAL_DMA_STATE_READY && OnTransferComplete)
    {
        OnTransferComplete();
    }
}
