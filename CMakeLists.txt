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

set(ST_DEVICE STM32F446xx)

set(HSE_VALUE 24000000)
set(LSE_VALUE 32768)

#############################################################
#                         Drivers                           #
#############################################################

add_library(CMSIS_CONFIG INTERFACE)
target_compile_definitions(CMSIS_CONFIG INTERFACE -D${ST_DEVICE}
                                                    -DHSE_VALUE=${HSE_VALUE}
                                                    -DLSE_VALUE=${LSE_VALUE})

add_library(STM_HAL_CONFIG INTERFACE)
target_include_directories(STM_HAL_CONFIG INTERFACE include)

add_library(STM_USB_CONFIG INTERFACE)
target_include_directories(STM_USB_CONFIG INTERFACE include/usb_dev)

add_subdirectory(Drivers)

#############################################################
#                        FreeRTOS                           #
#############################################################

set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE include) 
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

add_subdirectory(FreeRTOS-Kernel)

#############################################################
#                        BusCardProj                        #
#############################################################

get_target_property(CMSIS_SOURCES CMSIS SOURCES)
get_target_property(STM_USB_SOURCES STM_USB SOURCES)

add_subdirectory(interfaces)
add_subdirectory(nes_emu)
target_link_libraries(NES_EMU INTERFACES_LIB)
target_include_directories(NES_EMU PRIVATE 
                                    include
                                    include/Gui
                                    include/Utils)

set(TARGET ${PROJECT_NAME}.elf)

file(GLOB SOURCES
    source/*.cpp
    source/*.c
    source/usb_dev/*.c
    source/Drivers/*.cpp
)

add_executable(${TARGET} main.cpp ${SOURCES} ${CMSIS_SOURCES} ${STM_USB_SOURCES})

target_compile_definitions(${TARGET} PUBLIC
                            -DVERSION="${PROJECT_VERSION}"
                            -DDATE="${DATE}")

target_link_libraries(${TARGET} INTERFACES_LIB CMSIS STM_HAL STM_USB freertos_kernel NES_EMU)

target_include_directories(${TARGET} PUBLIC 
                            include 
                            include/usb_dev)

target_link_options(${TARGET} PUBLIC 
                        -T  ${PROJECT_SOURCE_DIR}/STM32F446RETX_FLASH.ld
                        -Wl,-Map=${PROJECT_NAME}.map
                        -Wl,--defsym,_Stack_Size=0x1000
                        -Wl,--defsym,_Heap_Size=0x1000)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${TARGET} ${PROJECT_NAME}.bin)

install(FILES build/${PROJECT_NAME}.bin DESTINATION ./ RENAME ${PROJECT_NAME}_${PROJECT_VERSION}.bin)