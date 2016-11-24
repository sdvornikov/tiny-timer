cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME arduinoClion)
project(${PROJECT_NAME})
register_hardware_platform(/Users/sergey/Development/avr/platforms/ATtiny)
#print_board_list()
#print_board_settings(mATtiny45)

set(${CMAKE_PROJECT_NAME}_BOARD mATtiny45)
set(${CMAKE_PROJECT_NAME}_PROGRAMMER arduinoasisp)
set(${CMAKE_PROJECT_NAME}_PORT /dev/cu.usbmodem1421)

set(${CMAKE_PROJECT_NAME}_SKETCH tinyClock.ino)
generate_arduino_firmware(${CMAKE_PROJECT_NAME} AFLAGS -v)