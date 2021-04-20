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

#
# Source files.
#

# Main executable sources.
set(EXAMPLE_SRCS
  ../minimal/main_minimal.cc
  app_browser_impl.cc
  client_impl.cc
  client_impl.h
  )
set(EXAMPLE_SRCS_WINDOWS
  resource_util_win_impl.cc
  )
APPEND_PLATFORM_SOURCES(EXAMPLE_SRCS)

if(OS_LINUX OR OS_WINDOWS)
  # On Windows and Linux the same executable is used for all processes.
  set(EXAMPLE_SRCS
    ${EXAMPLE_SRCS}
    ../minimal/app_other_minimal.cc
    app_renderer_impl.cc
    )
elseif(OS_MAC)
  # On macOS a separate helper executable is used for subprocesses.
  set(EXAMPLE_HELPER_SRCS
    ../minimal/app_other_minimal.cc
    ../minimal/process_helper_mac_minimal.cc
    app_renderer_impl.cc
    )
endif()

# Main executable resources.
set(EXAMPLE_RESOURCES_SRCS
  resources/message_router.html
  )
set(EXAMPLE_RESOURCES_SRCS_WINDOWS
  # Resources that embed "message_router.html" in the executable.
  resources/win/resource.h
  resources/win/resource.rc
  )
APPEND_PLATFORM_SOURCES(EXAMPLE_RESOURCES_SRCS)

if(OS_MACOSX OR OS_WINDOWS)
  # On macOS and Windows include the message_router and shared resources.
  set(EXAMPLE_RESOURCES_SRCS
    ${EXAMPLE_RESOURCES_SRCS}
    ${SHARED_RESOURCES_SRCS}
    )
endif()


#
# Shared configuration.
#

# Target executable names.
set(EXAMPLE_TARGET "message_router")
if(OS_MAC)
  set(EXAMPLE_HELPER_TARGET "message_router_Helper")
  set(EXAMPLE_HELPER_OUTPUT_NAME "message_router Helper")
endif()


#
# Linux configuration.
#

if(OS_LINUX)
  # Executable target.
  add_executable(${EXAMPLE_TARGET} ${EXAMPLE_SRCS})
  SET_EXAMPLE_EXECUTABLE_TARGET_PROPERTIES(${EXAMPLE_TARGET})
  add_dependencies(${EXAMPLE_TARGET} shared)
  target_link_libraries(${EXAMPLE_TARGET} shared)

  # Copy resource files to the target output directory.
  COPY_FILES("${EXAMPLE_TARGET}" "${EXAMPLE_RESOURCES_SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}" "${EXAMPLE_TARGET_OUT_DIR}/${EXAMPLE_TARGET}_files")
endif()


#
# Mac OS X configuration.
#

if(OS_MAC)

  # Create source groups for Xcode.
  SET_EXAMPLE_SOURCE_GROUPS("${EXAMPLE_SRCS}")
  SET_EXAMPLE_SOURCE_GROUPS("${EXAMPLE_HELPER_SRCS}")

  # Output path for the main app bundle.
  set(EXAMPLE_APP "${EXAMPLE_TARGET_OUT_DIR}/${EXAMPLE_TARGET}.app")

  # Variables referenced from the main Info.plist file.
  set(EXECUTABLE_NAME "${EXAMPLE_TARGET}")
  set(PRODUCT_NAME "${EXAMPLE_TARGET}")

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
  endif()

  # Main app bundle target.
  add_executable(${EXAMPLE_TARGET} MACOSX_BUNDLE ${EXAMPLE_SRCS} ${EXAMPLE_RESOURCES_SRCS} ${LOOT_GUI_HEADERS} ${LOOT_GUI_SRC})
  SET_EXAMPLE_EXECUTABLE_TARGET_PROPERTIES(${EXAMPLE_TARGET})
  add_dependencies(${EXAMPLE_TARGET} shared ${HugeLootDep})
  target_link_libraries(${EXAMPLE_TARGET} shared ${HugeLootLib})
  set_target_properties(${EXAMPLE_TARGET} PROPERTIES
    RESOURCE "${EXAMPLE_RESOURCES_SRCS}"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_INFO_PLIST}"
    )

  # Copy the CEF framework into the Frameworks directory.
  add_custom_command(
    TARGET ${EXAMPLE_TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${EXAMPLE_APP}/Contents/Frameworks/Chromium Embedded Framework.framework"
    VERBATIM
    )

  # Create the multiple Helper app bundle targets.
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    # Convert to a list and extract the suffix values.
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)
    list(GET _suffix_list 2 _plist_suffix)

    # Define Helper target and output names.
    set(_helper_target "${EXAMPLE_HELPER_TARGET}${_target_suffix}")
    set(_helper_output_name "${EXAMPLE_HELPER_OUTPUT_NAME}${_name_suffix}")

    # Create Helper-specific variants of the helper-Info.plist file. Do this
    # manually because the configure_file command (which is executed as part of
    # MACOSX_BUNDLE_INFO_PLIST) uses global env variables and would insert the
    # wrong values with multiple targets.
    set(_helper_info_plist "${CMAKE_CURRENT_BINARY_DIR}/helper-Info${_target_suffix}.plist")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_HELPER_INFO_PLIST}" _plist_contents)
    string(REPLACE "\${EXECUTABLE_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${PRODUCT_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${BUNDLE_ID_SUFFIX}" "${_plist_suffix}" _plist_contents ${_plist_contents})
    file(WRITE ${_helper_info_plist} ${_plist_contents})

    # Create Helper executable target.
    add_executable(${_helper_target} MACOSX_BUNDLE ${EXAMPLE_HELPER_SRCS})
    SET_EXAMPLE_EXECUTABLE_TARGET_PROPERTIES(${_helper_target})
    add_dependencies(${_helper_target} shared_helper)
    target_link_libraries(${_helper_target} shared_helper)
    set_target_properties(${_helper_target} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST ${_helper_info_plist}
      OUTPUT_NAME ${_helper_output_name}
      )

    if(USE_SANDBOX)
      target_link_libraries(${_helper_target} cef_sandbox_lib)
    endif()

    # Add the Helper as a dependency of the main executable target.
    add_dependencies(${EXAMPLE_TARGET} "${_helper_target}")

    # Copy the Helper app bundle into the Frameworks directory.
    add_custom_command(
      TARGET ${EXAMPLE_TARGET}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${EXAMPLE_TARGET_OUT_DIR}/${_helper_output_name}.app"
              "${EXAMPLE_APP}/Contents/Frameworks/${_helper_output_name}.app"
      VERBATIM
      )
  endforeach()

  # Manually process and copy over resource files.
  # The Xcode generator can support this via the set_target_properties RESOURCE
  # directive but that doesn't properly handle nested resource directories.
  # Remove these prefixes from input file paths.
  set(PREFIXES
    "../shared/resources/mac/"
    )
  COPY_MAC_RESOURCES("${EXAMPLE_RESOURCES_SRCS}" "${PREFIXES}" "${EXAMPLE_TARGET}" "${CMAKE_CURRENT_SOURCE_DIR}" "${EXAMPLE_APP}")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
    # Add resources to the sources variable for convenience.
  set(EXAMPLE_SRCS
    ${EXAMPLE_SRCS}
    ${EXAMPLE_RESOURCES_SRCS}
    )

  # Create source groups for Visual Studio.
  SET_EXAMPLE_SOURCE_GROUPS("${EXAMPLE_SRCS}")

  # Executable target.
  add_executable(${EXAMPLE_TARGET} WIN32 ${EXAMPLE_SRCS})
  SET_EXAMPLE_EXECUTABLE_TARGET_PROPERTIES(${EXAMPLE_TARGET})
  add_dependencies(${EXAMPLE_TARGET} shared)
  target_link_libraries(${EXAMPLE_TARGET} shared)
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
    get_target_property (EXISTING_LINK_FLAGS ${EXAMPLE_TARGET} LINK_FLAGS)
    IF (EXISTING_LINK_FLAGS)
        set (LOOT_LINK_FLAGS "${EXISTING_LINK_FLAGS} ${LOOT_LINK_FLAGS}")
    ENDIF ()
    set_target_properties (${EXAMPLE_TARGET} PROPERTIES LINK_FLAGS ${LOOT_LINK_FLAGS})
ENDIF ()

##############################
# Post-Build Steps
##############################

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(
        TARGET ${EXAMPLE_TARGET}
        POST_BUILD
        COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/src/gui/LOOT.exe.manifest\" -inputresource:"$(TargetDir)$(TargetFileName)"\;\#1 -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
        COMMENT "Adding manifest..."
    )
ENDIF ()

# Copy CEF binaries.
FOREACH(cef_dll ${CEF_PRECOMPILED_BINARIES})
    add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CEF_PRECOMPILED_BINARIES_DIR}/${cef_dll}"
            $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>/${cef_dll})
ENDFOREACH()

# Copy CEF resources.
FOREACH(cef_resource ${CEF_RESOURCES})
    add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CEF_RESOURCES_DIR}/${cef_resource}"
            $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>/${cef_resource})
ENDFOREACH()

# Copy CEF en-US locale resource.
add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CEF_LOCALES_DIR}/${CEF_LOCALE_FILE}"
        $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>/resources/l10n/${CEF_LOCALE_FILE})

# Copy the API binary to the build directory.
add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIBLOOT_EXTRACTED_PATH}/${LIBLOOT_SHARED_LIBRARY}"
        "$<TARGET_FILE_DIR:${EXAMPLE_TARGET}>/${LIBLOOT_SHARED_LIBRARY}")
add_custom_command(TARGET loot_gui_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIBLOOT_EXTRACTED_PATH}/${LIBLOOT_SHARED_LIBRARY}"
        "$<TARGET_FILE_DIR:loot_gui_tests>/${LIBLOOT_SHARED_LIBRARY}")

# Build the UI HTML.
add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
    COMMAND yarn build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Copy testing plugins
ExternalProject_Get_Property(testing-plugins SOURCE_DIR)
add_custom_command(TARGET loot_gui_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}
        $<TARGET_FILE_DIR:loot_gui_tests>)
