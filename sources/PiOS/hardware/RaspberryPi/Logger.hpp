//
// Created by dawid on 8/28/17.
//

#ifndef PROJECT_LOGGER_HPP
#define PROJECT_LOGGER_HPP


#include <Uart/Uart.hpp>

namespace Logger {

    uart::OutUart& log() {
        uart::cout << "[LOG]: ";
        return uart::clog;
    }

}


#endif //PROJECT_LOGGER_HPP
