project(AutomaticsControlExample)

enable_language(ASM)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        main.cpp
        ImageData.cpp
        ImageData.hpp
        )

set(DEPENDENCIES
        PiOS
        AllocationOperators
        )

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "C") #Just fuckin' do not link this libstdc++ horror
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Weffc++")
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPENDENCIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -D ${PROJECT_NAME} > ${PROJECT_NAME}.list
        COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME} -O binary ${PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME} -O ihex ${PROJECT_NAME}.hex)