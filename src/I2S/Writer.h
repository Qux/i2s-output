#ifndef WRITER_H
#define WRITER_H

#pragma once

#include <Arduino.h>

/* C++ */
#include <array>

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "esp_system.h"

#include "Config.h"
#include "Types.h"
#include "App/ListeningApp.hpp"
#include "Util/Static_FIFO.h"
#include "Util/Math_Utils.h"

namespace I2S {
    class Writer {
        public:
            Writer(ListeningApp* _app);
            void begin();

            inline void setAudioBufferPtr(Types::fifobuffer_t* _buf) {
                buf = _buf;
            };

            friend void audioWriteTask(void* param);
            void updateBuffer();

        private:            
            ListeningApp* app;
            TaskHandle_t writerTaskHandle;
            QueueHandle_t queue;
            Types::fifobuffer_t* buf;      
            // Types::fifobuffer_t& buffer;
    };

    void audioWriteTask(void* param);
}


#endif // !WRITER_H



