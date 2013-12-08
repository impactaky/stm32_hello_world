set (LIB_PATH ../Libraries)

set(SRC_DIRS 
  .
	../usb_src
	../system_src
  ${LIB_PATH}/STM32_USB_OTG_Driver/src
  ${LIB_PATH}/STM32_USB_Device_Library/Core/src
  ${LIB_PATH}/STM32_USB_Device_Library/Class/cdc/src
  )

include_directories(
  .
	../usb_src
	../system_src
  ${LIB_PATH}/CMSIS/Device/ST/STM32F4xx/Include
  ${LIB_PATH}/CMSIS/Include
  ${LIB_PATH}/STM32F4xx_StdPeriph_Driver/inc
  ${LIB_PATH}/STM32_USB_Device_Library/Class/cdc/inc
  ${LIB_PATH}/STM32_USB_Device_Library/Core/inc
  ${LIB_PATH}/STM32_USB_OTG_Driver/inc
  )

foreach(SRC_DIR ${SRC_DIRS})
  file(GLOB SRC ${SRC} "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.s")
endforeach(SRC_DIR)

link_directories (${LIB_PATH}/STM32F4xx_StdPeriph_Driver) 
add_executable(${CMAKE_PROJECT_NAME}.elf ${SRC})
target_link_libraries (${CMAKE_PROJECT_NAME}.elf STM32F4xx_StdPeriph_Driver) 

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.tmp
  COMMAND ${CC}-objcopy -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
  DEPENDS ${CMAKE_PROJECT_NAME}.elf)
add_custom_target(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.tmp) 
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_PROJECT_NAME}.bin)

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.tmp2
  COMMAND ${CC}-objdump -S ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.lst
  DEPENDS ${CMAKE_PROJECT_NAME}.elf)
add_custom_target(${CMAKE_PROJECT_NAME}.lst ALL DEPENDS ${CMAKE_PROJECT_NAME}.tmp2) 
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_PROJECT_NAME}.lst)
