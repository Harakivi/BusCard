#ifndef _ITCLASES_H
#define _ITCLASES_H

class NMI
{
public:
    static void Handler();
};

class HardFault
{
public:
    static void Handler();
};

class MemManage
{
public:
    static void Handler();
};

class BusFault
{
public:
    static void Handler();
};

class UsageFault
{
public:
    static void Handler();
};

class SVC
{
public:
    static void Handler();
};

class DebugMon
{
public:
    static void Handler();
};

class PendSV
{
public:
    static void Handler();
};

class SystemTick
{
public:
    static void Handler();
};

/////////////////////////////
//// External Interrupts ////
/////////////////////////////

class WWDG_IRQ
{
public:
    static void Handler();
};

class PVD_IRQ
{
public:
    static void Handler();
};

class TAMPER_IRQ
{
public:
    static void Handler();
};

class RTC_IRQ
{
public:
    static void Handler();
};

class FLASH_IRQ
{
public:
    static void Handler();
};

class RCC_IRQ
{
public:
    static void Handler();
};

class EXTI0_IRQ
{
public:
    static void Handler();
};

class EXTI1_IRQ
{
public:
    static void Handler();
};

class EXTI2_IRQ
{
public:
    static void Handler();
};
class EXTI3_IRQ
{
public:
    static void Handler();
};

class EXTI4_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream0_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream1_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream2_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream3_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream4_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream5_IRQ
{
public:
    static void Handler();
};

class DMA1_Stream6_IRQ
{
public:
    static void Handler();
};

class ADC_IRQ
{
public:
    static void Handler();
};

class EXTI9_5_IRQ
{
public:
    static void Handler();
};

class TIM1_BRK_TIM9_IRQ
{
public:
    static void Handler();
};

class TIM1_UP_TIM10_IRQ
{
public:
    static void Handler();
};

class TIM1_TRG_COM_TIM11_IRQ
{
public:
    static void Handler();
};

class TIM1_CC_IRQ
{
public:
    static void Handler();
};

class TIM2_IRQ
{
public:
    static void Handler();
};

class TIM3_IRQ
{
public:
    static void Handler();
};

class TIM4_IRQ
{
public:
    static void Handler();
};

class I2C1_EV_IRQ
{
public:
    static void Handler();
};

class I2C1_ER_IRQ
{
public:
    static void Handler();
};

class I2C2_EV_IRQ
{
public:
    static void Handler();
};

class I2C2_ER_IRQ
{
public:
    static void Handler();
};

class SPI1_IRQ
{
public:
    static void Handler();
};

class SPI2_IRQ
{
public:
    static void Handler();
};

class USART1_IRQ
{
public:
    static void Handler();
};

class USART2_IRQ
{
public:
    static void Handler();
};

class SDIO_IRQ
{
public:
    static void Handler();
};

class TIM5_IRQ
{
public:
    static void Handler();
};

class SPI3_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream0_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream1_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream2_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream3_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream4_IRQ
{
public:
    static void Handler();
};

class OTG_FS_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream5_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream6_IRQ
{
public:
    static void Handler();
};

class DMA2_Stream7_IRQ
{
public:
    static void Handler();
};

class USART6_IRQ
{
public:
    static void Handler();
};

class I2C3_EV_IRQ
{
public:
    static void Handler();
};

class I2C3_ER_IRQ
{
public:
    static void Handler();
};

class FPU_IRQ
{
public:
    static void Handler();
};

class SPI4_IRQ
{
public:
    static void Handler();
};

class SPI5_IRQ
{
public:
    static void Handler();
};

//------------------------------------------------
#endif /* _ITCLASES_H */