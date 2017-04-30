//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXTINTERFACE_HPP
#define PROJECT_CONTEXTINTERFACE_HPP

namespace PiOS {

    template<typename T>
    class has_startContext_method {
        template<typename T1>
        static char testMethod(decltype(&T1::startContext));

        template<typename T1>
        static long long testMethod(...);
    public:
        enum {
            value = sizeof testMethod<T>(nullptr) == 1
        };
    };

    template<typename T>
    class has_saveContext_method {
        template<typename T1>
        static char testMethod(decltype(&T1::saveContext));

        template<typename T1>
        static long long testMethod(...);

    public:
        enum {
            value = sizeof testMethod<T>(nullptr) == 1
        };
    };
}


#endif //PROJECT_CONTEXTINTERFACE_HPP
