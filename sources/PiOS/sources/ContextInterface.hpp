//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXTINTERFACE_HPP
#define PROJECT_CONTEXTINTERFACE_HPP

namespace PiOS {
    class ContextInterface {
    public:
        virtual void startContext()=0;

        virtual void saveContext()=0;

        virtual ~ContextInterface() {};
    };
}


#endif //PROJECT_CONTEXTINTERFACE_HPP
