#ifndef ADC_H
#define ADC_H

#pragma once

/*
- ADC hardware configration
*/



/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"


/* User Generated */
#include "Config.h"

namespace ADC {
    void setup();
}


#endif // !ADC_H_


