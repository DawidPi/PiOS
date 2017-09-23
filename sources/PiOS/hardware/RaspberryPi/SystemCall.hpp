//
// Created by dapl on 2017-05-04.
//

#ifndef PROJECT_SYSTEMCALL_HPP
#define PROJECT_SYSTEMCALL_HPP

#include <cassert>
#include <Pi.hpp>
#include "../../sources/CommonTypes.hpp"

namespace PiOS {

    class SystemCall {
    public:
        void operator()(SystemCallType sysCall);
    };

}

#endif //PROJECT_SYSTEMCALL_HPP
