cmake_minimum_required(VERSION 3.20)
project(BusCard LANGUAGES C CXX ASM)

set(ST_DEVICE STM32F411xE)

set(HSE_VALUE 25000000)
set(LSE_VALUE 32768)

#############################################################
#                          CMSIS                            #
#############################################################

string(TOLOWER ${ST_DEVICE} ST_DEVICE_LOWERCASE)
string(TOUPPER ${ST_DEVICE} ST_DEVICE_UPPERCASE)

file(GLOB CMSIS_SRC
    Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c
    Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_${ST_DEVICE_LOWERCASE}.s
)

add_library(CMSIS_LIBRARY STATIC ${CMSIS_SRC})

target_include_directories(CMSIS_LIBRARY PUBLIC Drivers/CMSIS/Include/ Drivers/CMSIS/Device/ST/STM32F4xx/Include/)

target_compile_definitions(CMSIS_LIBRARY PUBLIC 
                            -D${ST_DEVICE}
                            -DHSE_VALUE=${HSE_VALUE}
                            -DLSE_VALUE=${LSE_VALUE})

target_link_options(CMSIS_LIBRARY PRIVATE -nostdlib)

get_target_property(CMSIS_LIBRARY_INCLUDE_DIR CMSIS_LIBRARY INCLUDE_DIRECTORIES)

#############################################################
#                          STM_HAL                          #
#############################################################

if(ST_DEVICE STREQUAL STM32F411xE OR ST_DEVICE STREQUAL STM32F100x8)
set(ST_DEVICE_HAL STM32F100xB)
else()
set(ST_DEVICE_HAL ST_DEVICE)
endif()

file(GLOB STM_HAL_SRC
    Drivers/STM_HAL/Src/*.c
)

add_library(STM_HAL_LIBRARY STATIC ${STM_HAL_SRC})

target_include_directories(STM_HAL_LIBRARY PUBLIC 
                            Drivers/STM_HAL/Inc/ 
                            Include/ 
                            ${CMSIS_LIBRARY_INCLUDE_DIR})

target_compile_definitions(STM_HAL_LIBRARY PUBLIC 
                            -D${ST_DEVICE}
                            -DHSE_VALUE=${HSE_VALUE}
                            -DLSE_VALUE=${LSE_VALUE})

target_link_libraries(STM_HAL_LIBRARY CMSIS_LIBRARY)

target_link_options(STM_HAL_LIBRARY PRIVATE -nostdlib)

get_target_property(STM_HAL_LIBRARY_INCLUDE_DIR STM_HAL_LIBRARY INCLUDE_DIRECTORIES)

#############################################################
#                       STM32_USB                           #
#############################################################

file(GLOB_RECURSE STM_USB_SRC
    Drivers/STM32_USB/Class/MSC/*.c
    Drivers/STM32_USB/Core/*.c
)

add_library(STM_USB_LIBRARY STATIC ${STM_USB_SRC})

target_include_directories(STM_USB_LIBRARY PUBLIC 
                            Drivers/STM32_USB/Class/MSC/Inc/
                            Drivers/STM32_USB/Class/MSC/Inc/
                            Drivers/STM32_USB/Core/Inc/
                            Include/usb_dev/)

target_link_libraries(STM_USB_LIBRARY STM_HAL_LIBRARY)

target_link_options(STM_USB_LIBRARY PRIVATE -nostdlib)

get_target_property(STM_USB_INCLUDE_DIR STM_USB_LIBRARY INCLUDE_DIRECTORIES)

#############################################################
#                        BusCardProj                        #
#############################################################

set(TARGET ${PROJECT_NAME}.elf)

file(GLOB SOURCES
    Source/*.cpp
    Source/usb_dev/*.c
)

add_executable(${TARGET} main.cpp ${SOURCES})

target_link_libraries(${TARGET} CMSIS_LIBRARY STM_HAL_LIBRARY STM_USB_LIBRARY)

target_include_directories(${TARGET} PUBLIC STM_USB_INCLUDE_DIR STM_HAL_LIBRARY_INCLUDE_DIR CMSIS_LIBRARY_INCLUDE_DIR)

target_link_options(${TARGET} PUBLIC 
                        -T  ${PROJECT_SOURCE_DIR}/STM32F411CEUX_FLASH.ld
                        -Wl,-Map=${PROJECT_NAME}.map
                        -Wl,--defsym,_Stack_Size=0x400
                        -Wl,--defsym,_Heap_Size=0x200)