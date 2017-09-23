//
// Created by dawid on 25.06.17.
//

#ifndef PROJECT_UART_HPP
#define PROJECT_UART_HPP

extern "C" void uart_flush();
extern "C" void uart_send(unsigned int character);
extern "C" unsigned int uart_read();
extern "C" bool uart_dataToRead();

#endif //PROJECT_UART_HPP
