# The name of our project is "HELLO". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.8.1) 
project (stm32_test) 
enable_language(ASM)
set(CC arm-none-eabi)  

include_directories(src)


set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -DHSE_VALUE=80000000 -Wall -fmessage-length=0 -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O3 -g3 -gdwarf-2")
set(CMAKE_C_FLAGS   "-DUSE_FULL_ASSERT=1 -ffreestanding -DUSE_USB_OTG_FS=1 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=80000000 -O3 -Wall -fmessage-length=0 -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2")
set(CMAKE_CXX_FLAGS "-DUSE_FULL_ASSERT=1 -ffreestanding -DUSE_USB_OTG_FS=1 -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=80000000 -O3 -Wall -fno-exceptions -fno-rtti -fmessage-length=0 -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -std=c++0x")
# -DUSE_STM32F4_DISCOVERY 
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" /usr/local/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_ASM_COMPILER ${CC}-gcc)
set(CMAKE_C_COMPILER ${CC}-gcc)
set(CMAKE_CXX_COMPILER ${CC}-g++)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/stm32f4_flash.ld -Wl,--gc-sections -Wl,--build-id=none -fstack-usage -mlittle-endian -ffreestanding -nostartfiles -Wl,-Map,${CMAKE_PROJECT_NAME}.map -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O3 -g3 -gdwarf-2 ")
#

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)
#set_target_properties(STM32F4xx_StdPeriph_Driver PROPERTIES LINKER_LANGUAGE CXX)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Libraries/STM32F4xx_StdPeriph_Driver/src) 
