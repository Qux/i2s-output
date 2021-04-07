#pragma once

#include <Arduino.h>

#include <iostream>

#include <cstddef>
#include <functional>
#include <vector>
#include <utility>
// #include <thread>

#include "esp_timer.h"

#include "App/ListeningApp.hpp"
#include "Event.h"


namespace Control {

    class Metro {
    public:         
        // template<class T>        
        // void add( std::function<void()> f, const std::size_t interval, const Resolution r = Millis);
        
        template<class Ta, class Tb>
        void add( Ta* obj, void (Tb::*f)(), const std::size_t interval, const Time_Unit r = Millis);

        // template<typename... Ts>
        // void addT(std::function<Ts> f, const std::size_t interval = 100, const Resolution r = Millis) {};
        void update();                        
    private:
        static inline unsigned int now(const Time_Unit r) {
            switch (r)  {
                case Millis:
                    return millis();
                    break;
                case Micros:
                    return micros();
                    break;
                default:
                    return millis();
                    break;
            }
        };
      
        std::vector<Event> events;

    };

    template<class Ta, class Tb>
    inline void Metro::add( Ta* obj, void (Tb::*f)(), const std::size_t interval, const Time_Unit r ) {
        // const MetroEvent event {f, r, interval, now(r)};
        // eve.func();
        // events.push_back(std::move(eve));

        // auto func = [=]() {
        //     return (obj->*f);
        // };
        //   (obj->*f)();

        func_pair args;
        args.obj = obj;
        args.f = f;
      
        esp_timer_handle_t _timer;
        esp_timer_create_args_t _timerConfig;
        _timerConfig.arg = reinterpret_cast<void*>(arg);
        _timerConfig.callback = Metro::bang;
        _timerConfig.dispatch_method = ESP_TIMER_TASK;
        _timerConfig.name = "Ticker";
        if (_timer) {
            esp_timer_stop(_timer);
            esp_timer_delete(_timer);
        }
        esp_timer_create(&_timerConfig, &_timer);
        if (true) {
            esp_timer_start_periodic(_timer, interval * 1000ULL);
        } else {
            esp_timer_start_once(_timer, interval * 1000ULL);
        }
    }

    inline void Metro::update() {
        for(auto event : events) {
            const auto currentTime = now(event.resolution);
            if( ( currentTime - event.previousTime) >= event.interval) {
                (*event.func)(NULL);
                event.previousTime = currentTime;
            }
        }
    }

};