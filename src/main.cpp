#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_system.h"


#include <cmath>
#include <array>
#include <limits>
#include <iostream>
#include <functional>

#include "Config.h"
#include "DSP/DelayLine.h"
#include "DSP/Oscillator.h"
#include "UnitTest.h"
#include "Util/Static_FIFO.h"

typedef std::array<int32_t, Config::Buffer_Length * Config::Channels> audiobuf_t;

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

void unit_test() {
    using namespace UnitTest;
    std::cout << "Beginning unit test..." << std::endl;
    DelayLine<int, 10> idelay;    
    assert_equal(idelay.get(0), 0);
    idelay.add(1);    
    assert_equal(idelay.get(0), 1);
    idelay.add(2);
    assert_equal(idelay.get(0), 2);
    assert_equal(idelay.get(1), 1);

    std::cout << "Float Delay testing..." << std::endl;

    DelayLine<float, 10> fdelay;    
    assert_equal(fdelay.get(0), 0.0, true);
    fdelay.add(1.0);
    assert_equal(fdelay.get(0), 1.0);
    fdelay.add(2.0);
    assert_equal(fdelay.get(0), 2.0);
    assert_equal(fdelay.get(1), 1.0);
    assert_equal(fdelay.get(2), 0.0);
    assert_equal(fdelay.get(3), 0.0);
    assert_equal(fdelay.get(4), 0.0);

    std::cout << "finished Unit test " << std::endl;
}


/*
float audioSample() {
    // static int freq = 440;
    static Oscillator osc;
    float val = osc.getNext();
    osc.setFreq(freq);
    return val;
}

void dsp() {
    // static double phase = 0.0;
    static std::array<int32_t, Config::Buffer_Length> sample_buffer;

    constexpr int numLoop = sample_buffer.size() / 2;
    for(int i = 0; i < numLoop; i++) {        
        const uint32_t val = static_cast<int32_t>(audioSample() * Config::range) << 8;  
                
        sample_buffer[i * 2] = val;
        sample_buffer[i * 2 + 1] = val;
        // phase += Config::phase_per_sample;
        // if (1.0 <= phase) {
        //     phase = 0.0;
        // } 
        // step_count++;
    }
    
    size_t i2s_bytes_write = 0;    
    i2s_write(Config::ADC::I2S_NUM, sample_buffer.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
}
*/

void dsp(float& ch0, float& ch1) {
    
}


void process_audio() {
    static std::array<int32_t, Config::Buffer_Length * Config::Channels> buf;
    std::size_t i2s_bytes_read = 0;
    i2s_read(Config::ADC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);

    for(std::size_t i = 0; i < Config::Buffer_Length; i++) {
        dsp(static_cast<float>(buf[2 * i]) * Config::Bit_Range_Reciprocal, static_cast<float>(buf[2 * i + 1]) * Config::Bit_Range_Reciprocal);
    }
    
    size_t i2s_bytes_write = 0;        
    i2s_write(Config::DAC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
}


void vAudioReadLoop(Static_FIFO<audiobuf_t, Config::Buffer_Count>& buf) {
    while(true) {
        if(buf.available()) {
            audiobuf_t tmpbuf;

            std::size_t i2s_bytes_read = 0;
            i2s_read(Config::ADC::I2S_NUM, tmpbuf.data(), Config::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);

            buf.push(tmpbuf);
        }
    }
}

void vAudioWriteLoop(Static_FIFO<audiobuf_t, Config::Buffer_Count>& buf) {
    while(true) {
        if(buf.count() != 0) {
            audiobuf_t& samples = buf.pop();

            // DSP HERE

            size_t i2s_bytes_write = 0;        
            i2s_write(Config::DAC::I2S_NUM, samples.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
        }
    }
}

void setup_adc() {
    i2s_config_t i2s_config {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_SLAVE | I2S_MODE_RX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,  //I2S_COMM_FORMAT_STAND_MSB - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = 4,
        .dma_buf_len = Config::Buffer_Length * Config::Channels, //64,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0, //Config::Sampling_Rate * 256,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::ADC::Pins::BCK,
        .ws_io_num = Config::ADC::Pins::WS,
        .data_out_num = Config::ADC::Pins::DO,  //Not used
        .data_in_num = Config::ADC::Pins::DI               
    };
    i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::ADC::I2S_NUM, &pin_config);
    i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}

void setup_dac() {
    //for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample)
    //depend on bits_per_sample
    //using 6 buffers, we need 60-samples per buffer
    //if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes    16bit (= 2byte) * 2ch = 4  
    //if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes
    i2s_config_t i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S,  //I2S_COMM_FORMAT_STAND_MSB - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = 4,
        .dma_buf_len = Config::Buffer_Length * Config::Channels,//64,   samples
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::DAC::Pins::BCK,
        .ws_io_num = Config::DAC::Pins::WS,
        .data_out_num = Config::DAC::Pins::DO,
        .data_in_num = Config::DAC::Pins::DI              //Not used
    };
    i2s_driver_install(Config::DAC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::DAC::I2S_NUM, &pin_config);
    i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}

void setup_i2s() {
    std::cout << "Initializing ADC..." << std::endl;
    setup_adc();
    std::cout << "Initializing DAC..." << std::endl;
    setup_dac();
}

void setup_clock() {
    const ledc_channel_t channel = LEDC_CHANNEL_0;
    const std::size_t freq = 12288000;
    const int gpio_pin = 18; 

    ledc_timer_config_t ledc_timer {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_1_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = freq,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = gpio_pin,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,        
        .duty = 1,
        .hpoint = 0,
    };
    ledc_channel_config(&ledc_channel);
    // ledc_fade_func_install(0);
    // ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_NO_WAIT);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 1);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);   
}



void vAudioLoop(void* param) {    
    while(true) {
        process_audio();
        // dsp();
        // vPortYield();
        // vTaskDelay(1);
    }
}

void vControlLoop(void* param) {
    constexpr TickType_t interval = 1000 / portTICK_PERIOD_MS;
    while (true)    {
        vTaskDelay(interval);        
    }
}

// void setup() {
//     unit_test();
//     setup_i2s();
//     xTaskCreatePinnedToCore(vControlLoop, "ControlLoop", 4096, NULL, 2, NULL, 1);
//     xTaskCreatePinnedToCore(vAudioLoop, "AudioLoop", 4096, NULL, 1, NULL, 0);
// }

// void loop() {

// }


extern "C" void app_main(void) {
    // initArduino();
    setup_clock();
    setup_i2s();

    
    xTaskCreatePinnedToCore(vControlLoop, "ControlLoop", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vAudioLoop, "AudioLoop", 4096, NULL, 1, NULL, 0);
    // xTaskCreatePinnedToCore(vAudioReadLoop, "Audio Read Loop", 4096, NULL, 2, NULL, 0);
    // xTaskCreatePinnedToCore(vAudioReadLoop, "Audio Read Loop", 4096, NULL, 2, NULL, 0);
    constexpr TickType_t interval = 1000 / portTICK_PERIOD_MS;
    while (1) {
        vTaskDelay(interval);
    }
}
