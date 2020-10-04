#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include <cmath>
#include <array>
#include <limits>

#include "Config.h"
#include "Oscillator.h"

// #define SAMPLE_RATE     (48000)
#define I2S_NUM         (0)
#define WAVE_FREQ_HZ    (100)
#define PI              (3.1415926535)
#define I2S_BCK_IO      (GPIO_NUM_13)
#define I2S_WS_IO       (GPIO_NUM_15)
#define I2S_DO_IO       (GPIO_NUM_21)
#define I2S_DI_IO       (-1)

constexpr int  SAMPLE_PER_CYCLE = Config::Sampling_Rate/WAVE_FREQ_HZ;

extern "C" {
    void app_main(void);
    //static void setup_triangle_sine_waves(int bits);
}

/*
static void setup_triangle_sine_waves(int bits) {   
    constexpr int len = ((24+8)/16)*SAMPLE_PER_CYCLE*4;
    int *samples_data = (int *)malloc(((bits+8)/16)*SAMPLE_PER_CYCLE*4);    // What are these: 8, 16, 4? (bits+8)/16 detects either 16 bit or 32 bit and 4 = 4byte = size of int * 2ch?
    unsigned int i, sample_val;
    double sin_float, triangle_float, triangle_step = (double) pow(2, bits) / SAMPLE_PER_CYCLE;
    size_t i2s_bytes_write = 0;

    printf("\r\nTest bits=%d free mem=%d, written data=%d\n", bits, esp_get_free_heap_size(), static_cast<int>(((bits+8)/16)*SAMPLE_PER_CYCLE*4));

    triangle_float = -(std::pow(2, bits)/2 - 1);

    for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
        sin_float = sin(i * 2 * M_PI / SAMPLE_PER_CYCLE);
        if(sin_float >= 0)
            triangle_float += triangle_step;
        else
            triangle_float -= triangle_step;

        sin_float *= (std::pow(2, bits)/2 - 1);

        if (bits == 16) {
            sample_val = 0;
            sample_val += (short)triangle_float;
            sample_val = sample_val << 16;
            sample_val += (short) sin_float;
            samples_data[i] = sample_val;
        } else if (bits == 24) { //1-bytes unused
            samples_data[i*2] = ((int) triangle_float) << 8;
            samples_data[i*2 + 1] = ((int) sin_float) << 8;
        } else {
            samples_data[i*2] = ((int) triangle_float);
            samples_data[i*2 + 1] = ((int) sin_float);
        }

    }

    // i2s_set_clk(I2S_NUM, SAMPLE_RATE, bits, 2);

    // i2s_set_clk(I2S_NUM, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
    i2s_set_clk(I2S_NUM, SAMPLE_RATE, static_cast<i2s_bits_per_sample_t>(bits), I2S_CHANNEL_STEREO);
    //Using push
    // for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
    //     if (bits == 16)
    //         i2s_push_sample(0, &samples_data[i], 100);
    //     else
    //         i2s_push_sample(0, &samples_data[i*2], 100);
    // }
    // or write
    i2s_write(I2S_NUM, samples_data, ((bits+8)/16)*SAMPLE_PER_CYCLE*4, &i2s_bytes_write, 100);
    

    free(samples_data);
}
*/

// Oscillator osc;


volatile int freq = 440;

float audioSample(const unsigned int step_count) {
    // static int freq = 440;
    static Oscillator osc;
    float val = osc.getNext();
    osc.setFreq(freq);
    return val;
}

void dsp(unsigned int& step_count) {
    // static double phase = 0.0;
    static std::array<int32_t, Config::Buffer_Length> sample_buffer;

    constexpr int numLoop = sample_buffer.size() / 2;
    for(int i = 0; i < numLoop; i++) {        
        const uint32_t val = static_cast<uint32_t>(audioSample(step_count) * Config::range) << 8;
                
        sample_buffer[i * 2] = val;
        sample_buffer[i * 2 + 1] = val;
        // phase += Config::phase_per_sample;
        // if (1.0 <= phase) {
        //     phase = 0.0;
        // } 
        // step_count++;
    }
    
    size_t i2s_bytes_write = 0;    
    i2s_write(I2S_NUM, sample_buffer.data(), Config::I2S_Write_Size, &i2s_bytes_write, portMAX_DELAY);
}

void setupI2S() {
    //for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample)
    //depend on bits_per_sample
    //using 6 buffers, we need 60-samples per buffer
    //if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes    16bit (= 2byte) * 2ch = 4  
    //if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes
    i2s_config_t i2s_config = {
            .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),                                   // Only TX
            .sample_rate = Config::Sampling_Rate,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
            .communication_format = I2S_COMM_FORMAT_I2S,  //I2S_COMM_FORMAT_STAND_MSB - probably version thing
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
            .dma_buf_count = 8,
            .dma_buf_len = Config::Buffer_Length,//64,
            .use_apll = true,
            .tx_desc_auto_clear = false,
            .fixed_mclk = 0,
        };
        i2s_pin_config_t pin_config = {
            .bck_io_num = I2S_BCK_IO,
            .ws_io_num = I2S_WS_IO,
            .data_out_num = I2S_DO_IO,
            .data_in_num = I2S_DI_IO                                               //Not used
        };
        i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
        i2s_set_pin(I2S_NUM, &pin_config);
        i2s_set_clk(I2S_NUM, Config::Sampling_Rate, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_STEREO);
}

void vAudioLoop(void* param) {    
    unsigned int step_count = 0;
    while(1) {
        dsp(step_count);
        // vPortYield();
        // vTaskDelay(1);
    }
}

void vControlLoop(void* param) {
    constexpr TickType_t interval = 1000 / portTICK_PERIOD_MS;
    while (true)    {
        freq += 40;
        if(880 < freq) {
            freq = 440;
        }
        vTaskDelay(interval);        
    }
}

void app_main(void) {
    setupI2S();
    xTaskCreatePinnedToCore(vControlLoop, "ControlLoop", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vAudioLoop, "AudioLoop", 4096, NULL, 1, NULL, 0);
    while (1) {
        vTaskDelay(1);
    }
}
