//
// Created by dapl on 2017-04-27.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Time.hpp>
#include <RealTimeTask.hpp>
#include <Pi.hpp>
#include <SystemCall.hpp>
#include "Mocks.hpp"

using namespace PiOS;
using ::testing::_;
using ::testing::Return;

class PiOSTest : public ::testing::Test {
public:

    static bool taskExecuted;
};