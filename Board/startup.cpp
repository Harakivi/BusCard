#pragma language = extended
#pragma segment = "CSTACK"

extern "C" void __program_start(void);

#include "ItClases.h"

typedef void (*intfunc)(void);
typedef union
{
    intfunc __fun;
    void *__ptr;
} intvec_elem;

// The vector table is normally located at address 0.
// When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
// If you need to define interrupt service routines,
// make a copy of this file and include it in your project.
// The name "__vector_table" has special meaning for C-SPY:
// it is where the SP start value is found, and the NVIC vector
// table register (VTOR) is initialized to this address if != 0.

extern "C" const intvec_elem __vector_table[] @ ".intvec"=
    {
        {.__ptr = __sfe("CSTACK")},
        __program_start,

        NMI::Handler,
        HardFault::Handler,
        MemManage::Handler,
        BusFault::Handler,
        UsageFault::Handler,
        0,
        0,
        0,
        0,
        SVC::Handler,
        DebugMon::Handler,
        0,
        PendSV::Handler,
        SystemTick::Handler,

        /////////////////////////////
        //// External Interrupts ////
        /////////////////////////////
        WWDG_IRQ::Handler,
        PVD_IRQ::Handler,
        TAMPER_IRQ::Handler,
        RTC_IRQ::Handler,
        FLASH_IRQ::Handler,
        RCC_IRQ::Handler,
        EXTI0_IRQ::Handler,
        EXTI1_IRQ::Handler,
        EXTI2_IRQ::Handler,
        EXTI3_IRQ::Handler,
        EXTI4_IRQ::Handler,
        DMA1_Stream0_IRQ::Handler,
        DMA1_Stream1_IRQ::Handler,
        DMA1_Stream2_IRQ::Handler,
        DMA1_Stream3_IRQ::Handler,
        DMA1_Stream4_IRQ::Handler,
        DMA1_Stream5_IRQ::Handler,
        DMA1_Stream6_IRQ::Handler,
        ADC_IRQ::Handler,
        0,
        0,
        0,
        0,
        EXTI9_5_IRQ::Handler,
        TIM1_BRK_TIM9_IRQ::Handler,
        TIM1_UP_TIM10_IRQ::Handler,
        TIM1_TRG_COM_TIM11_IRQ::Handler,
        TIM1_CC_IRQ::Handler,
        TIM2_IRQ::Handler,
        TIM3_IRQ::Handler,
        TIM4_IRQ::Handler,
        I2C1_EV_IRQ::Handler,
        I2C1_ER_IRQ::Handler,
        I2C2_EV_IRQ::Handler,
        I2C2_ER_IRQ::Handler,
        SPI1_IRQ::Handler,
        SPI2_IRQ::Handler,
        USART1_IRQ::Handler,
        USART2_IRQ::Handler,
        0,
        SDIO_IRQ::Handler,
        TIM5_IRQ::Handler,
        SPI3_IRQ::Handler,
        0,
        0,
        0,
        0,
        DMA2_Stream0_IRQ::Handler,
        DMA2_Stream1_IRQ::Handler,
        DMA2_Stream2_IRQ::Handler,
        DMA2_Stream3_IRQ::Handler,
        DMA2_Stream4_IRQ::Handler,
        0,
        0,
        0,
        0,
        0,
        0,
        OTG_FS_IRQ::Handler,
        DMA2_Stream5_IRQ::Handler,
        DMA2_Stream6_IRQ::Handler,
        DMA2_Stream7_IRQ::Handler,
        USART6_IRQ::Handler,
        I2C3_EV_IRQ::Handler,
        I2C3_ER_IRQ::Handler,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        FPU_IRQ::Handler,
        0,
        0,
        SPI4_IRQ::Handler,
        SPI5_IRQ::Handler,
};

__weak void NMI::Handler()
{
    while (1)
    {
    }
}
__weak void HardFault::Handler()
{
    while (1)
    {
    }
}
__weak void MemManage::Handler()
{
    while (1)
    {
    }
}
__weak void BusFault::Handler()
{
    while (1)
    {
    }
}
__weak void UsageFault::Handler()
{
    while (1)
    {
    }
}
__weak void SVC::Handler()
{
    while (1)
    {
    }
}
__weak void DebugMon::Handler()
{
    while (1)
    {
    }
}
__weak void PendSV::Handler()
{
    while (1)
    {
    }
}
__weak void SystemTick::Handler()
{
    while (1)
    {
    }
}
__weak void WWDG_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void PVD_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TAMPER_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void RTC_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void FLASH_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void RCC_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI0_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI1_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI3_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI4_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream0_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream1_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream3_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream4_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream5_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA1_Stream6_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void ADC_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void EXTI9_5_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM1_BRK_TIM9_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM1_UP_TIM10_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM1_TRG_COM_TIM11_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM1_CC_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM3_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM4_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C1_EV_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C1_ER_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C2_EV_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C2_ER_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SPI1_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SPI2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void USART1_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void USART2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SDIO_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void TIM5_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SPI3_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream0_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream1_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream2_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream3_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream4_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void OTG_FS_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream5_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream6_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void DMA2_Stream7_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void USART6_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C3_EV_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void I2C3_ER_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void FPU_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SPI4_IRQ::Handler()
{
    while (1)
    {
    }
}
__weak void SPI5_IRQ::Handler()
{
    while (1)
    {
    }
}

extern "C" void __cmain(void);
extern "C" __weak void SystemInit();

#pragma required = __vector_table
void __program_start(void)
{
    SystemInit();
    __cmain();
}
