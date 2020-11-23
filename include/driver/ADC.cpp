#include "ADC.h"

void ADC::setup() {
    i2s_config_t i2s_config {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_SLAVE | I2S_MODE_RX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::ADC::DMA::Buffer_Count,
        .dma_buf_len = Config::ADC::DMA::Buffer_Length, //64,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = Config::Sampling_Rate * 256,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::ADC::Pins::BCK,
        .ws_io_num = Config::ADC::Pins::WS,
        .data_out_num = Config::ADC::Pins::DO,  //Not used
        .data_in_num = Config::ADC::Pins::DI               
    };
    i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::ADC::I2S_NUM, &pin_config);
    i2s_set_sample_rates(Config::ADC::I2S_NUM, Config::Sampling_Rate);
    i2s_zero_dma_buffer(Config::ADC::I2S_NUM);
    i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}