#ifndef WRITER_H
#define WRITER_H

#pragma once

#include <array>

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "esp32/spiram.h"

/* User Generated */
#include "Config.h"
#include "Types.h"
#include "Util/Static_FIFO.h"
#include "Util/Math_Utils.h"

#include "DSP/Oscillator.h"

// void DSP(float& Lch, float& Rch);

namespace I2S {
    class Writer {
    private:
        TaskHandle_t writerTaskHandle;
        QueueHandle_t queue;
        Types::audiobuf_t* buf;
        

    public:
        void begin();
        Oscillator osc;
        inline void setAudioBufferPtr(Types::audiobuf_t* _buf) {
            buf = _buf;
        };

        inline Types::audiobuf_t* getAudioBufferPtr() const {
            return buf;
        }

        inline QueueHandle_t getQueue() const {
            return queue;
        }
    };
}


#endif // !WRITER_H



