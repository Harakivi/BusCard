cmake_minimum_required(VERSION 3.20)
project(BusCard LANGUAGES C CXX ASM)

#======================================================#
#                   Version.h                          #
#======================================================#
if(NOT DEFINED GITHUB_RUN_NUMBER)
    set(GITHUB_RUN_NUMBER 0)
endif()

set(PROJECT_VERSION 0.0.1.${GITHUB_RUN_NUMBER})

string(TIMESTAMP DATE "%d.%m.%y")
message("Build version = ${PROJECT_VERSION}\r\nBuild date = ${DATE}")

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
    Drivers/STM32_USB/Class/CompositeBuilder/*.c
    Drivers/STM32_USB/Class/MSC/*.c
    Drivers/STM32_USB/Class/CDC/*.c
    Drivers/STM32_USB/Core/*.c
)

add_library(STM_USB_LIBRARY STATIC ${STM_USB_SRC})

target_include_directories(STM_USB_LIBRARY PUBLIC
                            Drivers/STM32_USB/Class/CompositeBuilder/Inc
                            Drivers/STM32_USB/Class/MSC/Inc/
                            Drivers/STM32_USB/Class/CDC/Inc/
                            Drivers/STM32_USB/Core/Inc/
                            Include/usb_dev/)

target_link_libraries(STM_USB_LIBRARY STM_HAL_LIBRARY)

target_link_options(STM_USB_LIBRARY PRIVATE -nostdlib)

get_target_property(STM_USB_INCLUDE_DIR STM_USB_LIBRARY INCLUDE_DIRECTORIES)

#############################################################
#                        FreeRTOS                           #
#############################################################

set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE Include) 
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

add_subdirectory(FreeRTOS-Kernel)

get_target_property(FREERTOS_INCLUDE_DIR freertos_kernel INCLUDE_DIRECTORIES)

#############################################################
#                        BusCardProj                        #
#############################################################

set(TARGET ${PROJECT_NAME}.elf)

file(GLOB SOURCES
    Source/*.cpp
    Source/*.c
    Source/usb_dev/*.c
)

add_executable(${TARGET} main.cpp ${SOURCES})

target_compile_definitions(${TARGET} PUBLIC
                            -DVERSION="${PROJECT_VERSION}"
                            -DDATE="${DATE}")

target_link_libraries(${TARGET} CMSIS_LIBRARY STM_HAL_LIBRARY STM_USB_LIBRARY freertos_kernel freertos_config)

target_include_directories(${TARGET} PUBLIC STM_USB_INCLUDE_DIR STM_HAL_LIBRARY_INCLUDE_DIR CMSIS_LIBRARY_INCLUDE_DIR FREERTOS_INCLUDE_DIR Include Include/usb_dev Include/Tasks)

target_link_options(${TARGET} PUBLIC 
                        -T  ${PROJECT_SOURCE_DIR}/STM32F411CEUX_FLASH.ld
                        -Wl,-Map=${PROJECT_NAME}.map
                        -Wl,--defsym,_Stack_Size=0x1000
                        -Wl,--defsym,_Heap_Size=0x1000)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${TARGET} ${PROJECT_NAME}.bin)

install(FILES build/${PROJECT_NAME}.bin DESTINATION ./ RENAME ${PROJECT_NAME}_${PROJECT_VERSION}.bin)