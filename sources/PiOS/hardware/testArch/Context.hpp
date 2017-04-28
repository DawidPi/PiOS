//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXT_HPP
#define PROJECT_CONTEXT_HPP

#include "../../sources/ContextInterface.hpp"

#define TEST_CONTEXT

namespace PiOS_hardware {
    using namespace PiOS;

    class Context final : public ContextInterface {
    public:
        Context() {}

        virtual void startContext() override;

        virtual void saveContext() override;

    public:
        static bool mContextStarted;
        static bool mContextSaved;
    };
}


#endif //PROJECT_CONTEXT_HPP
