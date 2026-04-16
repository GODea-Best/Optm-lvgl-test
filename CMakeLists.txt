cmake_minimum_required(VERSION 3.12.4)
project(lvgl C CXX)

# Set the correct FreeRTOS port for your system (e.g., Posix for WSL)
set(FREERTOS_PORT GCC_POSIX CACHE STRING "Port for FreeRTOS on Posix environment")

set(LVGL_PRO_PROJECT_DIR
    ""
    CACHE PATH "Path to LVGL Pro Project folder")

option(USE_FREERTOS "Enable FreeRTOS" OFF) # Turn this on to enable FreeRTOS

if(USE_FREERTOS)
    message(STATUS "FreeRTOS is enabled")

    # FreeRTOS integration when USE_FREERTOS is enabled
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/config)
    target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

    # Add FreeRTOS as a subdirectory
    add_subdirectory(FreeRTOS)

    # FreeRTOS-specific include directories
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/include)
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix)
    include_directories(${PROJECT_SOURCE_DIR}/config)

    # Add FreeRTOS sources
    file(GLOB FREERTOS_SOURCES
        "${PROJECT_SOURCE_DIR}/FreeRTOS/*.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/MemMang/heap_4.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix/*.c"
    )
else()
    message(STATUS "FreeRTOS is disabled")
    set(FREERTOS_SOURCES "")  # No FreeRTOS sources if FreeRTOS is disabled
endif()

# Main include files of the project
include_directories(${PROJECT_SOURCE_DIR}/main/inc)

# src subdirectory include paths — lets all source files use plain #include "name.h"
# regardless of which subfolder they live in, with no source changes required.
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/fonts
    ${PROJECT_SOURCE_DIR}/src/screens
    ${PROJECT_SOURCE_DIR}/src/assets
    ${PROJECT_SOURCE_DIR}/src/ui
)

# Define options for LVGL with default values (OFF)
option(LV_USE_DRAW_SDL "Use SDL draw unit" OFF)
option(LV_USE_LIBPNG "Use libpng to decode PNG" OFF)
option(LV_USE_LIBJPEG_TURBO "Use libjpeg turbo to decode JPEG" OFF)
option(LV_USE_FFMPEG "Use libffmpeg to display video using lv_ffmpeg" OFF)
option(LV_USE_FREETYPE "Use freetype library" OFF)

# Set C and C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output path for the executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

# Find and include SDL2 library
find_package(SDL2 REQUIRED)

# Add compile definitions based on the selected options
add_compile_definitions($<$<BOOL:${LV_USE_DRAW_SDL}>:LV_USE_DRAW_SDL=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBPNG}>:LV_USE_LIBPNG=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBJPEG_TURBO}>:LV_USE_LIBJPEG_TURBO=1>)
add_compile_definitions($<$<BOOL:${LV_USE_FFMPEG}>:LV_USE_FFMPEG=1>)

# Disable LVGL demos and examples — they use widgets/fonts disabled in lv_conf.h
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add LVGL subdirectory
add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS})

set(MAIN_SOURCES
# assets
src/assets/mouse_cursor_icon.c
# hal
src/hal/hal.c
# fonts
src/fonts/lv_font_montserrat_light_7.c
src/fonts/lv_font_montserrat_light_12.c
src/fonts/lv_font_montserrat_light_16.c
src/fonts/lv_font_montserrat_regular_10.c
src/fonts/lv_font_montserrat_regular_12.c
src/fonts/lv_font_montserrat_regular_14.c
src/fonts/lv_font_montserrat_regular_16.c
src/fonts/lv_font_montserrat_regular_18.c
src/fonts/lv_font_montserrat_regular_20.c
src/fonts/lv_font_montserrat_regular_22.c
src/fonts/lv_font_montserrat_regular_24.c
src/fonts/lv_font_montserrat_semibold_12.c
src/fonts/lv_font_montserrat_semibold_16.c
src/fonts/lv_font_montserrat_semibold_20.c
src/fonts/lv_font_montserrat_semibold_22.c
src/fonts/lv_font_montserrat_semibold_24.c
src/fonts/lv_font_montserrat_bold_16.c
src/fonts/lv_font_montserrat_bold_18.c
src/fonts/lv_font_montserrat_bold_20.c
src/fonts/lv_font_montserrat_bold_22.c
src/fonts/lv_font_montserrat_bold_24.c
src/fonts/lv_font_montserrat_extrabold_16.c
src/fonts/lv_font_montserrat_extrabold_18.c
src/fonts/lv_font_montserrat_extrabold_20.c
src/fonts/lv_font_montserrat_extrabold_22.c
src/fonts/lv_font_montserrat_extrabold_24.c
# control panel (simulator)
src/control_panel.c
# screens
src/screens/screen_blow_prompt.c
src/screens/screen_blow_prompt_anim.c
src/screens/screen_diagnostic.c
src/screens/screen_free_restart.c
src/screens/screen_free_restart_bold.c
src/screens/screen_free_restart_icons.c
src/screens/screen_initial_card.c
src/screens/screen_initial_card_bold.c
src/screens/screen_initial_test.c
src/screens/screen_initial_test_icons.c
src/screens/screen_initial_test_pass.c
src/screens/screen_initial_test_pass_icons.c
src/screens/screen_invalid_sample.c
src/screens/screen_invalid_sample_bold.c
src/screens/screen_invalid_sample_icons.c
src/screens/screen_lockout.c
src/screens/screen_lockout_bold.c
src/screens/screen_lockout_icons.c
src/screens/screen_logo.c
src/screens/screen_optmg4_logo.c
src/screens/screen_retest_alert.c
src/screens/screen_retest_alert_bold.c
src/screens/screen_retest_alert_icons.c
src/screens/screen_sample_analysis.c
src/screens/screen_service_alert.c
src/screens/screen_service_alert_bold.c
src/screens/screen_service_alert_icons.c
src/screens/screen_service_warning.c
src/screens/screen_service_warning_bold.c
src/screens/screen_service_warning_icons.c
src/screens/screen_tamper_warn.c
src/screens/screen_test_error.c
src/screens/screen_test_error_bold.c
src/screens/screen_test_error_icons.c
src/screens/screen_test_pass_bold.c
src/screens/screen_test_pass_watch.c
src/screens/screen_usa_logo.c
src/screens/screen_usa_logo_3d.c
src/screens/screen_violation_lockout.c
src/screens/screen_violation_lockout_bold.c
src/screens/screen_violation_lockout_icons.c
src/screens/screen_violation_warning.c
src/screens/screen_violation_warning_bold.c
src/screens/screen_violation_warning_icons.c
src/screens/screen_warning_msg.c
)
set(MAIN_LIBS lvgl lvgl::thorvg ${SDL2_LIBRARIES})

# Create the main executable, depending on the FreeRTOS option
if(USE_FREERTOS)
    list(APPEND MAIN_SOURCES src/freertos_main.c src/freertos/freertos_posix_port.c ${FREERTOS_SOURCES})
    list(APPEND MAIN_LIBS freertos_config freertos_kernel)
else()
    list(APPEND MAIN_SOURCES src/main.c)
endif()


if(NOT MSVC)
    list(APPEND MAIN_LIBS m pthread)
endif()

# Custom target to run the executable
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

# Conditionally include and link SDL2_image if LV_USE_DRAW_SDL is enabled
if(LV_USE_DRAW_SDL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
    target_include_directories(lvgl PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
    list(APPEND MAIN_LIBS ${SDL2_IMAGE_LIBRARIES})
endif()

# Conditionally include and link libpng if LV_USE_LIBPNG is enabled
if(LV_USE_LIBPNG)
    find_package(PNG REQUIRED)
    target_include_directories(lvgl PUBLIC ${PNG_INCLUDE_DIRS})
    list(APPEND MAIN_LIBS ${PNG_LIBRARIES})
endif()

# Conditionally include and link libjpeg-turbo if LV_USE_LIBJPEG_TURBO is enabled
if(LV_USE_LIBJPEG_TURBO)
    find_package(JPEG REQUIRED)
    target_include_directories(lvgl PUBLIC ${JPEG_INCLUDE_DIRS})
    list(APPEND MAIN_LIBS ${JPEG_LIBRARIES})
endif()

# Conditionally include and link FFmpeg libraries if LV_USE_FFMPEG is enabled
if(LV_USE_FFMPEG)
    list(APPEND MAIN_LIBS avformat avcodec avutil swscale z)
endif()

# Conditionally include and link FreeType if LV_USE_FREETYPE is enabled
if(LV_USE_FREETYPE)
    find_package(Freetype REQUIRED)
    target_include_directories(lvgl PUBLIC ${FREETYPE_INCLUDE_DIRS})
    list(APPEND MAIN_LIBS ${FREETYPE_LIBRARIES})
endif()

if(LVGL_PRO_PROJECT_DIR)
  if(EXISTS "${LVGL_PRO_PROJECT_DIR}/CMakeLists.txt")
    add_subdirectory(${LVGL_PRO_PROJECT_DIR} ui)
    list(APPEND MAIN_LIBS lib-ui)

    target_compile_definitions(lib-ui PUBLIC LV_LVGL_H_INCLUDE_SIMPLE)
    target_include_directories(lib-ui PUBLIC
      ${LVGL_PRO_PROJECT_DIR}  # local headers like ui.h, generated files, etc.
      ${CMAKE_SOURCE_DIR}/lvgl # include "lvgl.h" works
    )
    message(STATUS "LVGL Pro Project enabled from: ${LVGL_PRO_PROJECT_DIR}")
  else()
    message(
      WARNING
        "LVGL_PRO_PROJECT_DIR specified but no CMakeLists.txt found at: ${LVGL_PRO_PROJECT_DIR}"
    )
  endif()
endif()

add_executable(main ${MAIN_SOURCES})
target_compile_definitions(main PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main ${MAIN_LIBS})

# On Windows, GUI applications do not show a console by default,
# which hides log output. This ensures a console is available for logging.
if (WIN32)
    if (MSVC)
        target_link_options(main PRIVATE "/SUBSYSTEM:CONSOLE")
    else()
        target_link_options(main PRIVATE "-mconsole")
    endif()
endif()

# Release build: optimise for size+speed and strip dead code at link time
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lvgl PRIVATE -O2 -ffunction-sections -fdata-sections)
    target_compile_options(main  PRIVATE -O2 -ffunction-sections -fdata-sections)
    if(NOT MSVC)
        target_link_options(main PRIVATE -Wl,--gc-sections)
    endif()
endif()

# Apply additional compile options if the build type is Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode enabled")

    target_compile_options(lvgl PRIVATE
        -pedantic-errors
        -Wall
        -Wclobbered
        -Wdeprecated
        -Wdouble-promotion
        -Wempty-body
        -Wextra
        -Wformat-security
        -Wmaybe-uninitialized
        # -Wmissing-prototypes
        -Wpointer-arith
        -Wmultichar
        -Wno-pedantic # ignored for now, we convert functions to pointers for properties table.
        -Wreturn-type
        -Wshadow
        -Wshift-negative-value
        -Wsizeof-pointer-memaccess
        -Wtype-limits
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wfloat-conversion
        -Wstrict-aliasing
    )

if (ASAN)
    message(STATUS "AddressSanitizer enabled")

    # Add AddressSanitizer flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
else()
    message(STATUS "AddressSanitizer disabled")
endif()
endif()
