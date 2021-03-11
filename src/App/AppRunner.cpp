#include "AppRunner.hpp"

using namespace Types;

AppRunner::AppRunner(DeepListening* _app) : app{_app} {
    this->buf = new fifobuffer_t;
}

void AppRunner::initReader() {
    std::cout << "Reader begin" << std::endl;
    reader = new I2S::Reader(app);
    reader->setBufferPtr(buf);
    reader->begin();
}

void AppRunner::initWriter() {
    std::cout << "Writer begin" << std::endl;

    writer = new I2S::Writer(app);
    writer->setAudioBufferPtr(buf);
    writer->begin();
}

void AppRunner::init() {
    app->setup();
    switch (Config::Stream) {
        case Use_InOut:
            initReader();
            initWriter();
            break;

        case Input_Only:
            initReader();
            break;

        case Output_Only:
            initWriter();
            break;
        case No_Audio:
            break;
    }
}

void AppRunner::run() {
    this->init();


    while (true) {                
        app->control();
        vTaskDelay(Config::Control_Interval);
    }
}

