# Create a library called "Hello" which includes the source file "hello.cxx". 
# The extension is already found. Any number of sources could be listed here. 

#set(CURRENT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/STM32F4xx_StdPeriph_Driver/)

set(SRC_DIRS ../src)
include_directories(
  ../../CMSIS/STM32F4xx_StdPeriph_Driver/inc
  ../../CMSIS/STM32F4xx/Include
  ../inc
  ../../CMSIS/Device/ST/STM32F4xx/Include
  ../../CMSIS/Include
  )

foreach(SRC_DIR ${SRC_DIRS})
  file(GLOB SRC ${SRC} "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.s")
endforeach(SRC_DIR)

add_library (STM32F4xx_StdPeriph_Driver ${SRC})
