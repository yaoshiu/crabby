cmake_minimum_required(VERSION 3.23)

project(crabby)

file(GLOB
  SRC
  CONFIGURE_DEPENDS
  src/*.c
)
add_executable(test ${SRC})
