//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Time.hpp"
#include "DynamicAllocator.hpp"

namespace PiOS {
    /*!
     * Facade class, that allows to use Operating System.
     */
    class PiOS {
    public:
        PiOS(DynamicAllocator *memoryAllocator);

        bool createTask();

        void onTimeTick();

        void start();
    private:
    };
}


#endif //PROJECT_PIOS_HPP
