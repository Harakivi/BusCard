cmake_minimum_required(VERSION 3.20)

#############################################################
#                          CMSIS                            #
#############################################################

string(TOLOWER ${ST_DEVICE} ST_DEVICE_LOWERCASE)
string(TOUPPER ${ST_DEVICE} ST_DEVICE_UPPERCASE)

file(GLOB CMSIS_SRC
    CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c
    CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_${ST_DEVICE_LOWERCASE}.s
)

add_library(CMSIS INTERFACE ${CMSIS_SRC})

target_include_directories(CMSIS INTERFACE CMSIS/Include/ CMSIS/Device/ST/STM32F4xx/Include/)

target_link_libraries(CMSIS INTERFACE CMSIS_CONFIG)


#############################################################
#                          STM_HAL                          #
#############################################################

if(ST_DEVICE STREQUAL STM32F411xE OR ST_DEVICE STREQUAL STM32F100x8)
set(ST_DEVICE_HAL STM32F100xB)
else()
set(ST_DEVICE_HAL ST_DEVICE)
endif()

file(GLOB STM_HAL_SRC
    STM_HAL/Src/*.c
)

add_library(STM_HAL STATIC ${STM_HAL_SRC})

target_compile_options(STM_HAL PRIVATE -Ofast)

target_link_options(STM_HAL PRIVATE -nostdlib)

target_link_libraries(STM_HAL PRIVATE CMSIS STM_HAL_CONFIG)

target_include_directories(STM_HAL PUBLIC 
                            STM_HAL/Inc/)

#############################################################
#                       STM32_USB                           #
#############################################################

file(GLOB_RECURSE STM_USB_SRC
    STM32_USB/Class/CompositeBuilder/*.c
    STM32_USB/Class/MSC/*.c
    STM32_USB/Class/CDC/*.c
    STM32_USB/Core/*.c
)

add_library(STM_USB INTERFACE ${STM_USB_SRC})

target_include_directories(STM_USB INTERFACE
                            STM32_USB/Class/CompositeBuilder/Inc
                            STM32_USB/Class/MSC/Inc/
                            STM32_USB/Class/CDC/Inc/
                            STM32_USB/Core/Inc/)

target_link_libraries(STM_USB INTERFACE STM_USB_CONFIG)