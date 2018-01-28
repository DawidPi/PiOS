#!/bin/bash

mkdir ../versatilepb_build
cd ../versatilepb_build

cmake -DCMAKE_TOOLCHAIN_FILE='../toolchains/ARM/RaspberryPi/RaspberryPi2_toolchain.cmake' \
      -DPiOS_BUILD_TESTS=OFF \
      -DPiOS_BUILD_EXAMPLES=ON \
      -DPiOS_BUILD_NEW_OPERATOR=ON \
      -DPiOS_PLATFORM_TYPE=RaspberryPi \
      -G 'Unix Makefiles' \
      '../'