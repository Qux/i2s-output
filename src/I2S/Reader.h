#ifndef READER_H
#define READER_H

#pragma once

#include <Arduino.h>

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "esp_system.h"
// #include "esp32/spiram.h"


#include "Config.h"
#include "Types.h"
#include "DeepListening.hpp"


namespace I2S {
    class Reader {
        public:
            Reader(DeepListening* _app);
            void begin();
            void setBufferPtr(Types::fifobuffer_t* _buf) {
                buffer = _buf;
            }

            friend void audioReadTask(void *param);
        private:
            DeepListening* app;
            TaskHandle_t taskHandle;  // I2S reader task
            QueueHandle_t queue; // i2s reader queue
            Types::fifobuffer_t* buffer;
    };

    void audioReadTask(void* param);
}

#endif // !READER_H

