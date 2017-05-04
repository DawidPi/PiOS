//
// Created by dapl on 2017-05-04.
//

#ifndef PROJECT_SYSTEMCALL_HPP
#define PROJECT_SYSTEMCALL_HPP

#include "../../sources/CommonTypes.hpp"

namespace PiOS {

    class SystemCall {
    public:
        void operator()(SystemCallType sysCall) {
            if (sysCall == SystemCallType::EXIT)
                exitCalled = true;
        };
    public:
        static bool exitCalled;
    };

}

#endif //PROJECT_SYSTEMCALL_HPP
