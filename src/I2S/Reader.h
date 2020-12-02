#ifndef READER_H
#define READER_H

#pragma once

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "esp_system.h"
#include "esp32/spiram.h"

/* Self-defined stuff */
#include "Config.h"
#include "Types.h"
#include "DSP.h"

namespace I2S {
    class Reader {

        public:
            // Reader(Types::fifobuffer_t& _buf) : buffer(_buf){};
            void begin();
            inline QueueHandle_t getQueue() const {
                return queue;
            }
            void setBufferPtr(Types::fifobuffer_t* _buf) {
                buffer = _buf;
            }

            friend void audioReadTask(void *param);
        private:
            TaskHandle_t readerTaskHandle;  // I2S reader task
            TaskHandle_t writerTaskHandle;  // writer task        
            QueueHandle_t queue; // i2s reader queue
            // Types::fifobuffer_t& buffer;
            Types::fifobuffer_t* buffer;
    };

    void audioReadTask(void* param);
}

#endif // !READER_H

