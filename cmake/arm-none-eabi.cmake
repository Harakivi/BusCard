set(CMAKE_TOOLCHAIN_PREFIX arm-none-eabi-) 

find_program(CMAKE_ASM_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}gcc)
find_program(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}g++)
find_program(CMAKE_OBJCOPY ${CMAKE_TOOLCHAIN_PREFIX}objcopy)


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -Wall" CACHE INTERNAL "asm compiler flags")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall"  CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -std=c++20 -Wall" CACHE INTERNAL "cxx compiler flags")

set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS} -g3 -O0 -DDEBUG -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g3 -O0 -DDEBUG -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3 -O0 -DDEBUG -pedantic")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Ofast")

set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs -lc -lnosys -lm -Wl,--gc-sections -Wl,--print-memory-usage")