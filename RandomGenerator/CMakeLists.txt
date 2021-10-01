cmake_minimum_required(VERSION 3.5)

## project name
project(random_generator LANGUAGES CXX)

##set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCES
    trajectory.cpp
    locationinfo.cpp
    mqttservice.cpp
    main.cpp
)


find_package(Threads REQUIRED)

add_executable(random_generator ${SOURCES})

set_target_properties(random_generator PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

set_target_properties(random_generator PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(random_generator
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/paho/headers
)

target_link_libraries(random_generator  pthread)

target_link_libraries(random_generator paho-mqttpp3 paho-mqtt3as)
