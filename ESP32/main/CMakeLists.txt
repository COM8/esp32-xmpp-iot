cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(SOURCES EspIoT.cpp

            esp/RgbLed.cpp
            esp/WifiTask.cpp
            esp/bt/BluetoothServer.cpp
            esp/bt/BLECharacteristicRWCallback.cpp
            esp/bt/BLEServiceHelper.cpp
            
            esp/sensors/Bmp180.cpp)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} $ENV{IDF_PATH}/components
                       REQUIRES smooth_component bt cpp_utils)
