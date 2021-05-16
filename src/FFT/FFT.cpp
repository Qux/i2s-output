
#include "FFT.hpp"


FFT::FFT(const RAM_Type ram_type) {
    switch (ram_type)   {
        case RAM_Type::PSRAM:
            allocate(ps_calloc);
            // data = static_cast<float*>(ps_calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;

        case RAM_Type::SRAM:
            allocate(calloc);
            // data = static_cast<float*>(calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;        
        default:
            allocate(calloc);
            // data = static_cast<float*>(ps_calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;
    };

    counter = 0;
    init();
}

FFT::~FFT() {
    dsps_fft2r_deinit_fc32();
    deallocate();
}

void FFT::init() {
    esp_err_t error_state = 0;
    error_state = dsps_fft2r_init_fc32(NULL, Config::FFT::Size);    

    if(error_state != ESP_OK) {
        std::cout << "Failed to init FFT." << std::endl;
    }

    dsps_wind_hann_f32(window, Config::FFT::Size);
}

void FFT::calculate() {
    for(std::size_t i = 0; i < Config::FFT::Size; i++) {
        data[i * 2] = input[i] * window[i];
        data[i * 2 + 1] = 0.0;
    }

    dsps_fft2r_fc32_ansi(data, Config::FFT::Size);
    dsps_bit_rev_fc32_ansi(data, Config::FFT::Size);
    dsps_cplx2reC_fc32(data, Config::FFT::Size);

    
    for(std::size_t i = 0; i < (Config::FFT::Size >> 1); i++) {
        float re = data[i * 2];
        float im = data[i * 2 + 1];

        fft_result[i] = sqrt(re * re + im * im);
    }
}

void FFT::allocate(std::function<void*(size_t, size_t)> calloc_func) {
    input = static_cast<float*>(calloc_func(Config::FFT::Size * sizeof(float), sizeof(float)));
    data = static_cast<float*>(calloc_func(Config::FFT::Size * 2 * sizeof(float), sizeof(float)));
    window = static_cast<float*>(calloc_func(Config::FFT::Size * sizeof(float), sizeof(float)));
    fft_result = static_cast<float*>(calloc_func(Config::FFT::Size * sizeof(float), sizeof(float)));
}

void FFT::deallocate() {
    free(input);
    free(data);    
    free(window);
    free(fft_result);
}

