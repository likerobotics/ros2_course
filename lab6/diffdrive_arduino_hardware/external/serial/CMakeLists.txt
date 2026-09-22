cmake_minimum_required(VERSION 3.5)
project(serial_vendor)

# Собираем библиотеку serial из нужных исходников
add_library(serial STATIC
  src/serial.cc
  src/impl/unix.cc
  src/impl/list_ports/list_ports_linux.cc
)

target_include_directories(serial
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Для Linux нужны эти либы
if(UNIX AND NOT APPLE)
  find_package(Threads REQUIRED)
  target_link_libraries(serial
    PUBLIC
      Threads::Threads
    PRIVATE
      rt
  )
endif()