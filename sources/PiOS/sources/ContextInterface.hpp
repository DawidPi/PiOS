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

    template<typename T>
    class has_minStackSize_member {
        template<typename T1>
        static char testMethod(decltype(&T1::minStackSize));

        template<typename T1>
        static long long testMethod(...);

    public:
        enum {
            value = sizeof testMethod<T>(nullptr) == 1
        };
    };

    template<typename T>
    class implementsContextInterface {
    public:
        enum {
            value = has_saveContext_method<T>::value and
                    has_startContext_method<T>::value and
                    has_minStackSize_member<T>::value
        };
    };
}


#endif //PROJECT_CONTEXTINTERFACE_HPP
