#ifndef DAC_H
#define DAC_H

#pragma once

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"


/* User Generated */
#include "Config.h"

class DAC {
public:
    void setup();
    void begin();

};


#endif // !DAC_H
