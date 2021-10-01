cmake_minimum_required(VERSION 3.5)

project(data_processor LANGUAGES CXX)

set(SOURCES
    location.cpp
    parser.cpp
    dataanalizer.cpp
    sessionmanager.cpp
    main.cpp
)

find_package(Threads REQUIRED)

add_executable(data_processor ${SOURCES})

set_target_properties(data_processor PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )

set_target_properties(data_processor PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(data_processor
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/paho/headers
)

target_link_libraries(data_processor  pthread)

target_link_libraries(data_processor paho-mqttpp3 paho-mqtt3as)
