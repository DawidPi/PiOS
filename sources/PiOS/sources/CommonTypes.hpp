//
// Created by dapl on 2017-05-04.
//

#ifndef PROJECT_COMMONTYPES_HPP
#define PROJECT_COMMONTYPES_HPP

namespace PiOS {
    /*!
     * \brief Type for StartUp function functor.
     */
    using StartUp = void (*)();

    /*!
     * \brief Enum for System Calls Types
     */
    enum class SystemCallType {
        EXIT /*!< Exit system call. Responsible for ending currently processed task*/
    };
}

#endif //PROJECT_COMMONTYPES_HPP
