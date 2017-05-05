project(AllocationOperators)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        CustomNewDelete.cpp)


add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PiOS)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/sources/PiOS/sources")

install(TARGETS ${PROJECT_NAME} DESTINATION lib)