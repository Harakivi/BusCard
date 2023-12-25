#pragma once
#include "iSpi.hpp"
#include "Spi.h"

namespace Hardware
{
    template <int spi, bool useDma>
    class Spi : public iSpi
    {
    private:
        volatile bool _busy;
        static void OnTransferComplete();
        SpiHandler *_handler;
        Spi() : _busy(false), _handler(nullptr) {}

    public:
        static Spi &Get()
        {
            static Spi instance;
            return instance;
        }

        virtual bool Init(SpiHandler &handler)
        {
            if constexpr (spi == 1)
            {
                _handler = &handler;
                bool res = Spi1Init(OnTransferComplete);
                if constexpr (useDma)
                {
                    return res && Spi1DmaInit(OnTransferComplete);
                }
            }

            return false;
        }

        virtual void Write(uint8_t *buff, uint32_t size)
        {
            _busy = true;
            if constexpr (spi == 1)
            {
                if constexpr (useDma)
                    Spi1WriteDma(buff, size);
                else
                    Spi1Write(buff, size);
            }
        }

        virtual volatile bool IsBusy()
        {
            return _busy;
        }
    };

    template <int spi, bool useDma>
    void Spi<spi, useDma>::OnTransferComplete()
    {
        if (Spi::Get()._handler != nullptr)
            Spi::Get()._handler->onWriteDone();
        Spi::Get()._busy = false;
    }

}