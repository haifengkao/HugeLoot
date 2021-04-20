# Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

cmake_minimum_required (VERSION 3.6)
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0114 NEW) # Xcode "new build system"
cmake_policy(SET CMP0074 NEW) # find_package uses <PackageName>_ROOT variables, e.g. CEF_ROOT
project (HugeLOOT)
include(ExternalProject)

# options(case sesitive): release, debug
set (Active_Scheme debug)

option(MSVC_STATIC_RUNTIME "Build with static runtime libs (/MT)" OFF)

IF (${MSVC_STATIC_RUNTIME})
    set (MSVC_SHARED_RUNTIME OFF)
ELSE()
    set (MSVC_SHARED_RUNTIME ON)
ENDIF()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WinSampleDir win)

##############################
# OS Specific Setting
##############################
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(SYSTEM_NAME "Windows")
    set (Boost_USE_STATIC_LIBS ON)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(SYSTEM_NAME "Darwin")

    # a workaround for homebrew boost 1.75.0 https://github.com/Homebrew/homebrew-core/issues/67427#issuecomment-754141236
    set (Boost_USE_STATIC_LIBS OFF)

    # workaround because find_package(Boost) Could NOT find Threads
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)

ELSE ()
    set (Boost_USE_STATIC_LIBS ON)
    set(SYSTEM_NAME "Other")
ENDIF ()

set (Boost_USE_MULTITHREADED ON)


##############################
# Get Build Revision
##############################

find_package(Git)

IF (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_VARIABLE GIT_COMMIT_STRING
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
ELSE()
    SET (GIT_COMMIT_STRING "unknown")
ENDIF ()

message(STATUS "Git revision: ${GIT_COMMIT_STRING}")

# Write to file.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/gui/version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp" @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/generated")

##############################
# External Projects
##############################

IF (MSVC)
    set (Boost_USE_STATIC_RUNTIME ${MSVC_STATIC_RUNTIME})
ELSE()
    set (Boost_USE_STATIC_RUNTIME OFF)
ENDIF()

find_package(Boost REQUIRED COMPONENTS locale system)

if (SYSTEM_NAME MATCHES "Other")
    find_package(ICU REQUIRED COMPONENTS uc)
    include_directories($ICU_INCLUDE_DIRS)
    link_directories($ICU_LIBRARY_DIRS)
endif()

ExternalProject_Add(GTest
                    PREFIX "external"
                    URL "https://github.com/google/googletest/archive/release-1.8.1.tar.gz"
                    CMAKE_ARGS -Dgtest_force_shared_crt=${MSVC_SHARED_RUNTIME}
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(GTest SOURCE_DIR BINARY_DIR)
set (GTEST_INCLUDE_DIRS "${SOURCE_DIR}/googletest/include")
set (GTEST_LIBRARIES "${BINARY_DIR}/googlemock/gtest/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (NOT DEFINED LIBLOOT_URL)
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        if ("${CMAKE_GENERATOR_PLATFORM}" MATCHES "Win32")
            set(LIBLOOT_URL "https://github.com/loot/libloot/releases/download/0.16.2/libloot-0.16.2-0-gc74c16b_0.16.2-win32.7z")
        else()
            set(LIBLOOT_URL "https://github.com/loot/libloot/releases/download/0.16.2/libloot-0.16.2-0-gc74c16b_0.16.2-win64.7z")
        endif()
    else()
        set(LIBLOOT_URL "https://github.com/loot/libloot/releases/download/0.16.2/libloot-0.16.2-0-gc74c16b_0.16.2-Linux.tar.xz")
    endif()
endif()

ExternalProject_Add(json
                    PREFIX "external"
                    URL "https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp"
                    URL_HASH SHA256=7804b38146921d03374549c9e2a5e3acda097814c43caf2b96a0278e58df26e0
                    DOWNLOAD_NO_EXTRACT 1
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")
set(JSON_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/external/src")


set(LIBLOOT_BASE_PATH "../../../../BigLoot/")
ExternalProject_Add(libloot
                    PREFIX "external"
                    # URL ${LIBLOOT_URL}
                    SOURCE_DIR ${LIBLOOT_BASE_PATH}/build/${Active_Scheme}
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(libloot SOURCE_DIR)
set(LIBLOOT_EXTRACTED_PATH ${SOURCE_DIR})

# dirty directory path fix (why does them work?)
include_directories("../../../BigLoot/include")
link_directories("../../../BigLoot/build")

set(LIBLOOT_STATIC_LIBRARY "${CMAKE_STATIC_LIBRARY_PREFIX}loot${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(LIBLOOT_SHARED_LIBRARY "${CMAKE_SHARED_LIBRARY_PREFIX}loot${CMAKE_SHARED_LIBRARY_SUFFIX}")

ExternalProject_Add(testing-plugins
                    PREFIX "external"
                    URL "https://github.com/Ortham/testing-plugins/archive/1.4.1.tar.gz"
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")

ExternalProject_Add(cpptoml
                    PREFIX "external"
                    URL "https://github.com/skystrife/cpptoml/archive/v0.1.1.tar.gz"
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(cpptoml SOURCE_DIR)
set(CPPTOML_INCLUDE_DIRS "${SOURCE_DIR}/include")

ExternalProject_Add(spdlog
                    PREFIX "external"
                    URL "https://github.com/gabime/spdlog/archive/v1.8.5.tar.gz"
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND ""
                    INSTALL_COMMAND "")
ExternalProject_Get_Property(spdlog SOURCE_DIR)
set(SPDLOG_INCLUDE_DIRS "${SOURCE_DIR}/include")

##############################
# General Settings
##############################

set (LOOT_GUI_SRC "${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/main.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/helpers.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_handler.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_app.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_scheme_handler_factory.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/window_delegate.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/query_handler.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game_settings.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/helpers.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/logging.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_paths.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_settings.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_state.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/resource.rc")

set (LOOT_GUI_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/helpers.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_handler.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_app.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/loot_scheme_handler_factory.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/window_delegate.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/derived_plugin_metadata.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/json.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/query_executor.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/apply_sort_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/cancel_sort_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/change_game_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/clear_all_metadata_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/clear_plugin_metadata_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/clipboard_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/close_settings_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/copy_content_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/copy_load_order_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/copy_metadata_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/discard_unapplied_changes_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/editor_opened_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/editor_closed_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_auto_sort_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_conflicting_plugins_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_game_data_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_game_types_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_init_errors_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_installed_games_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_settings_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_themes_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/get_version_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/metadata_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/open_log_location_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/open_readme_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/redate_plugins_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/save_filter_state_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/save_user_groups_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/sort_plugins_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/types/update_masterlist_query.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/cef/query/query_handler.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game_detection_error.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game_settings.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/games_manager.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/helpers.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/logging.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_paths.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_settings.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_state.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/unapplied_change_counter.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/resource.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/version.h")

set(LOOT_GUI_TESTS_SRC "${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/helpers.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game_settings.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/helpers.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/logging.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_paths.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_settings.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_state.cpp"
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/main.cpp")

set (LOOT_GUI_TESTS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/helpers.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/game/game_settings.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_paths.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_settings.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/state/loot_state.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/cef/query/json_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/cef/query/types/close_settings_query_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/cef/query/types/editor_closed_query_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/cef/query/types/get_settings_query_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/cef/query/types/get_themes_query_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/game/game_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/game/game_settings_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/game/games_manager_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/game/helpers_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/loot_paths_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/loot_settings_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/state/unapplied_change_counter_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/helpers_test.h"
                            "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/gui/test_helpers.h")

source_group("Header Files\\gui" FILES ${LOOT_GUI_HEADERS})
source_group("Header Files\\tests" FILES ${LOOT_TESTS_HEADERS})
source_group("Header Files\\tests" FILES ${LOOT_GUI_TESTS_HEADERS})

source_group("Source Files\\gui" FILES ${LOOT_GUI_SRC})
source_group("Source Files\\tests" FILES ${LOOT_TESTS_SRC})
source_group("Header Files\\tests" FILES ${LOOT_GUI_TESTS_SRC})

# Include source and library directories.
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src"
                     ${CPPTOML_INCLUDE_DIRS}
                     ${JSON_INCLUDE_DIRS}
                     ${Boost_INCLUDE_DIRS}
                     ${GTEST_INCLUDE_DIRS}
                     ${SPDLOG_INCLUDE_DIRS})

##############################
# System-Specific Settings
##############################

# Settings when compiling for Windows.
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions (-DUNICODE -D_UNICODE -DNOMINMAX -DBOOST_UUID_FORCE_AUTO_LINK)
    set(LIBLOOT_LINK_LIBRARY ${LIBLOOT_STATIC_LIBRARY})
else()
    set(LIBLOOT_LINK_LIBRARY ${LIBLOOT_SHARED_LIBRARY})
endif()

IF(SYSTEM_NAME MATCHES "Darwin")
    # find_library(FoundationLib CoreFoundation)
    # find_library(SecurityLib Security)
    find_library(ICONVLib iconv)
    # set (LOOT_LIBS ${FoundationLib} ${SecurityLib} ${ICONVLib})
    set (LOOT_LIBS ${ICONVLib})
    set (LOOT_GUI_LIBS ${LOOT_LIBS})
    set (LOOT_TEST_LIBS ${LOOT_LIBS})
ELSEIF (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH};.")
    set (CMAKE_BUILD_WITH_INSTALL_RPATH ON)

    set (LOOT_LIBS pthread http_parser ssh2 stdc++fs icui18n)
    set (LOOT_GUI_LIBS X11 ${LOOT_LIBS})
    set (LOOT_TEST_LIBS ${LOOT_LIBS})
ENDIF ()

IF (MSVC)
    # Turn off permissive mode to be more standards-compliant and avoid compiler errors.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")

    # Set /bigobj to allow building Debug tests
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")

    # Update compiler flags.
    IF (MSVC_STATIC_RUNTIME)
        FOREACH(flag
            CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
            CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
            STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
            SET("${flag}" "${${flag}} /EHsc")
        ENDFOREACH()
    ENDIF ()

    set (LOOT_GUI_LIBS comctl32
                       Psapi)
ENDIF ()

##############################
# Common Build Setting
##############################
set(HugeLootDep libcef_dll_wrapper cpptoml json libloot spdlog)
set(HugeLootLib libcef_dll_wrapper ${CEF_STANDARD_LIBS} ${Boost_LIBRARIES} ${LIBLOOT_LINK_LIBRARY} ${LOOT_GUI_LIBS} ${ICU_LIBRARIES})

##############################
# Shared Example Codes
##############################
#
# Source files.
#

# Sources shared by all executables.
set(SHARED_COMMON_SRCS
  app_factory.h
  main_util.cc
  main_util.h
  )

# Main executable sources.
set(SHARED_SRCS
  ${SHARED_COMMON_SRCS}
  client_manager.cc
  client_manager.h
  client_util.cc
  client_util.h
  browser_util.cc
  browser_util.h
  main.h
  resource_util.cc
  resource_util.h
  )
set(SHARED_SRCS_LINUX
  client_util_linux.cc
  main_linux.cc
  resource_util_linux.cc
  resource_util_posix.cc
  )
set(SHARED_SRCS_MAC
  client_util_mac.mm
  main_mac.mm
  resource_util_mac.mm
  resource_util_posix.cc
  )
set(SHARED_SRCS_WINDOWS
  client_util_win.cc
  main_win.cc
  resource_util_win.cc
  )
APPEND_PLATFORM_SOURCES(SHARED_SRCS)

if(OS_MAC)
  # On macOS a separate helper executable is used for subprocesses.
  set(SHARED_HELPER_SRCS
    ${SHARED_COMMON_SRCS}
    process_helper_mac.cc
    process_helper_mac.h
    )
endif()


#
# Shared configuration.
#

# Target static library names.
set(SHARED_TARGET "shared")
if(OS_MAC)
  set(SHARED_HELPER_TARGET "shared_helper")
endif()

if(OS_LINUX OR OS_WINDOWS)
  # Logical target used to link the libcef library on Linux and Windows.
  # On macOS the CEF framework is loaded dynamically at startup.
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()


#
# Linux configuration.
#

if(OS_LINUX)
  # Static library target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${SHARED_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
  COPY_FILES("${SHARED_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")

  # Output a message about setting SUID permissions on the chrome-sandbox target.
  SET_LINUX_SUID_PERMISSIONS("${SHARED_TARGET}" "${EXAMPLE_TARGET_OUT_DIR}/chrome-sandbox")
endif()


#
# Mac OS X configuration.
#

if(OS_MAC)
  # Create source groups for Xcode.
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_SRCS}")
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_HELPER_SRCS}")

  # Helper static library target.
  add_library(${SHARED_HELPER_TARGET} STATIC ${SHARED_HELPER_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_HELPER_TARGET})
  add_dependencies(${SHARED_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_HELPER_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${SHARED_HELPER_TARGET} cef_sandbox_lib)
  endif()

  # Main static library target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS} ${LOOT_GUI_HEADERS} ${LOOT_GUI_SRC})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper "${SHARED_HELPER_TARGET}" ${HugeLootDep})
  target_link_libraries(${SHARED_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS} ${HugeLootLib})
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Create source groups for Visual Studio.
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_SRCS}")

  # Executable target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${SHARED_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
  endif()

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${SHARED_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
  COPY_FILES("${SHARED_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
endif()

##############################
# Define Targets
##############################

# Build application tests.
add_executable       (loot_gui_tests ${LOOT_GUI_TESTS_SRC} ${LOOT_GUI_TESTS_HEADERS})
add_dependencies     (loot_gui_tests cpptoml libloot spdlog GTest testing-plugins)
target_link_libraries(loot_gui_tests ${Boost_LIBRARIES} ${LIBLOOT_LINK_LIBRARY} ${GTEST_LIBRARIES} ${LOOT_TEST_LIBS} ${ICU_LIBRARIES})

##############################
# Set Target-Specific Flags
##############################

IF (MSVC)
    set (LOOT_LINK_FLAGS "/SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE")
    get_target_property (EXISTING_LINK_FLAGS ${SHARED_TARGET} LINK_FLAGS)
    IF (EXISTING_LINK_FLAGS)
        set (LOOT_LINK_FLAGS "${EXISTING_LINK_FLAGS} ${LOOT_LINK_FLAGS}")
    ENDIF ()
    set_target_properties (${SHARED_TARGET} PROPERTIES LINK_FLAGS ${LOOT_LINK_FLAGS})
ENDIF ()

##############################
# Post-Build Steps
##############################

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(
        TARGET ${SHARED_TARGET}
        POST_BUILD
        COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/src/gui/LOOT.exe.manifest\" -inputresource:"$(TargetDir)$(TargetFileName)"\;\#1 -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
        COMMENT "Adding manifest..."
    )
ENDIF ()

# Copy CEF binaries.
FOREACH(cef_dll ${CEF_PRECOMPILED_BINARIES})
    add_custom_command(TARGET ${SHARED_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CEF_PRECOMPILED_BINARIES_DIR}/${cef_dll}"
            $<TARGET_FILE_DIR:${SHARED_TARGET}>/${cef_dll})
ENDFOREACH()

# Copy CEF resources.
FOREACH(cef_resource ${CEF_RESOURCES})
    add_custom_command(TARGET ${SHARED_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CEF_RESOURCES_DIR}/${cef_resource}"
            $<TARGET_FILE_DIR:${SHARED_TARGET}>/${cef_resource})
ENDFOREACH()

# Copy CEF en-US locale resource.
add_custom_command(TARGET ${SHARED_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CEF_LOCALES_DIR}/${CEF_LOCALE_FILE}"
        $<TARGET_FILE_DIR:${SHARED_TARGET}>/resources/l10n/${CEF_LOCALE_FILE})

# Copy the API binary to the build directory.
add_custom_command(TARGET ${SHARED_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIBLOOT_EXTRACTED_PATH}/${LIBLOOT_SHARED_LIBRARY}"
        "$<TARGET_FILE_DIR:${SHARED_TARGET}>/${LIBLOOT_SHARED_LIBRARY}")
add_custom_command(TARGET loot_gui_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIBLOOT_EXTRACTED_PATH}/${LIBLOOT_SHARED_LIBRARY}"
        "$<TARGET_FILE_DIR:loot_gui_tests>/${LIBLOOT_SHARED_LIBRARY}")

# Build the UI HTML.
add_custom_command(TARGET ${SHARED_TARGET} POST_BUILD
    COMMAND yarn build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Copy testing plugins
ExternalProject_Get_Property(testing-plugins SOURCE_DIR)
add_custom_command(TARGET loot_gui_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}
        $<TARGET_FILE_DIR:loot_gui_tests>)