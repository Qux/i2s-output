#ifndef WRITER_H
#define WRITER_H

#pragma once

/* C++ */
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

namespace I2S {
    class Writer {
        public:
            // Writer(Types::fifobuffer_t& _buf) : buffer(_buf){};
            void begin();
            friend void audioWriteTask(void* param);
            inline void setAudioBufferPtr(Types::fifobuffer_t* _buf) {
                buf = _buf;
            };

            inline Types::fifobuffer_t* getAudioBufferPtr() const {
                return buf;
            }

            inline QueueHandle_t getQueue() const {
                return queue;
            }
        private:
            TaskHandle_t writerTaskHandle;
            QueueHandle_t queue;
            Types::fifobuffer_t* buf;      
            // Types::fifobuffer_t& buffer;
    };

    void audioWriteTask(void* param);
}


#endif // !WRITER_H



