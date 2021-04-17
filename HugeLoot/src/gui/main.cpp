/*  LOOT

    A load order optimisation tool for
    Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
    Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

    Copyright (C) 2014 WrinklyNinja

    This file is part of LOOT.

    LOOT is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    LOOT is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LOOT.  If not, see
    <https://www.gnu.org/licenses/>.
    */

/*
#include "gui/cef/loot_app.h"
#include "gui/state/loot_paths.h"

#ifdef _WIN32
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <include/cef_sandbox_win.h>
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <include/base/cef_logging.h>
#endif

CefSettings GetCefSettings(std::filesystem::path l10nPath) {
  CefSettings cef_settings;

  // Enable CEF command line args.
  cef_settings.command_line_args_disabled = false;

  // Disable CEF logging.
  cef_settings.log_severity = LOGSEVERITY_VERBOSE;

  // Load locale pack files from LOOT's l10n path.
  CefString(&cef_settings.locales_dir_path).FromString(l10nPath.u8string());

  return cef_settings;
}
#define BUILDING_CEF_SHARED 1
#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_command_line_capi.h"
#include "include/capi/cef_cookie_capi.h"
#include "include/capi/cef_crash_util_capi.h"
#include "include/capi/cef_drag_data_capi.h"
#include "include/capi/cef_file_util_capi.h"
#include "include/capi/cef_image_capi.h"
#include "include/capi/cef_media_router_capi.h"
#include "include/capi/cef_menu_model_capi.h"
#include "include/capi/cef_origin_whitelist_capi.h"
#include "include/capi/cef_parser_capi.h"
#include "include/capi/cef_path_util_capi.h"
#include "include/capi/cef_print_settings_capi.h"
#include "include/capi/cef_process_message_capi.h"
#include "include/capi/cef_process_util_capi.h"
#include "include/capi/cef_request_capi.h"
#include "include/capi/cef_request_context_capi.h"
#include "include/capi/cef_resource_bundle_capi.h"
#include "include/capi/cef_response_capi.h"
#include "include/capi/cef_scheme_capi.h"
#include "include/capi/cef_server_capi.h"
#include "include/capi/cef_ssl_info_capi.h"
#include "include/capi/cef_stream_capi.h"
#include "include/capi/cef_task_capi.h"
#include "include/capi/cef_thread_capi.h"
#include "include/capi/cef_trace_capi.h"
#include "include/capi/cef_urlrequest_capi.h"
#include "include/capi/cef_v8_capi.h"
#include "include/capi/cef_values_capi.h"
#include "include/capi/cef_waitable_event_capi.h"
#include "include/capi/cef_web_plugin_capi.h"
#include "include/capi/cef_xml_reader_capi.h"
#include "include/capi/cef_zip_reader_capi.h"
#include "include/capi/views/cef_browser_view_capi.h"
#include "include/capi/views/cef_display_capi.h"
#include "include/capi/views/cef_label_button_capi.h"
#include "include/capi/views/cef_menu_button_capi.h"
#include "include/capi/views/cef_panel_capi.h"
#include "include/capi/views/cef_scroll_view_capi.h"
#include "include/capi/views/cef_textfield_capi.h"
#include "include/capi/views/cef_window_capi.h"
#include "include/cef_api_hash.h"
#include "include/cef_version.h"
#include "include/internal/cef_logging_internal.h"
#include "include/internal/cef_string_list.h"
#include "include/internal/cef_string_map.h"
#include "include/internal/cef_string_multimap.h"
#include "include/internal/cef_string_types.h"
#include "include/internal/cef_thread_internal.h"
#include "include/internal/cef_time.h"
#include "include/internal/cef_trace_event_internal.h"
#include "include/wrapper/cef_library_loader.h"

typedef int (*cef_execute_process_ptr)(const struct _cef_main_args_t*,
                                       struct _cef_app_t*,
                                       void*);
typedef int (*cef_initialize_ptr)(const struct _cef_main_args_t*,
                                  const struct _cef_settings_t*,
                                  struct _cef_app_t*,
                                  void*);
typedef void (*cef_shutdown_ptr)();
typedef void (*cef_do_message_loop_work_ptr)();
typedef void (*cef_run_message_loop_ptr)();
typedef void (*cef_quit_message_loop_ptr)();
typedef void (*cef_set_osmodal_loop_ptr)(int);
typedef void (*cef_enable_highdpi_support_ptr)();
typedef int (*cef_crash_reporting_enabled_ptr)();
typedef void (*cef_set_crash_key_value_ptr)(const cef_string_t*,
                                            const cef_string_t*);
typedef int (*cef_create_directory_ptr)(const cef_string_t*);
typedef int (*cef_get_temp_directory_ptr)(cef_string_t*);
typedef int (*cef_create_new_temp_directory_ptr)(const cef_string_t*,
                                                 cef_string_t*);
typedef int (*cef_create_temp_directory_in_directory_ptr)(const cef_string_t*,
                                                          const cef_string_t*,
                                                          cef_string_t*);
typedef int (*cef_directory_exists_ptr)(const cef_string_t*);
typedef int (*cef_delete_file_ptr)(const cef_string_t*, int);
typedef int (*cef_zip_directory_ptr)(const cef_string_t*,
                                     const cef_string_t*,
                                     int);
typedef void (*cef_load_crlsets_file_ptr)(const cef_string_t*);
typedef int (*cef_add_cross_origin_whitelist_entry_ptr)(const cef_string_t*,
                                                        const cef_string_t*,
                                                        const cef_string_t*,
                                                        int);
typedef int (*cef_remove_cross_origin_whitelist_entry_ptr)(const cef_string_t*,
                                                           const cef_string_t*,
                                                           const cef_string_t*,
                                                           int);
typedef int (*cef_clear_cross_origin_whitelist_ptr)();
typedef int (*cef_parse_url_ptr)(const cef_string_t*, struct _cef_urlparts_t*);
typedef int (*cef_create_url_ptr)(const struct _cef_urlparts_t*, cef_string_t*);
typedef cef_string_userfree_t (*cef_format_url_for_security_display_ptr)(
    const cef_string_t*);
typedef cef_string_userfree_t (*cef_get_mime_type_ptr)(const cef_string_t*);
typedef void (*cef_get_extensions_for_mime_type_ptr)(const cef_string_t*,
                                                     cef_string_list_t);
typedef cef_string_userfree_t (*cef_base64encode_ptr)(const void*, size_t);
typedef struct _cef_binary_value_t* (*cef_base64decode_ptr)(
    const cef_string_t*);
typedef cef_string_userfree_t (*cef_uriencode_ptr)(const cef_string_t*, int);
typedef cef_string_userfree_t (*cef_uridecode_ptr)(const cef_string_t*,
                                                   int,
                                                   cef_uri_unescape_rule_t);
typedef struct _cef_value_t* (*cef_parse_json_ptr)(const cef_string_t*,
                                                   cef_json_parser_options_t);
typedef struct _cef_value_t* (
    *cef_parse_json_buffer_ptr)(const void*, size_t, cef_json_parser_options_t);
typedef struct _cef_value_t* (*cef_parse_jsonand_return_error_ptr)(
    const cef_string_t*,
    cef_json_parser_options_t,
    cef_string_t*);
typedef cef_string_userfree_t (*cef_write_json_ptr)(struct _cef_value_t*,
                                                    cef_json_writer_options_t);
typedef int (*cef_get_path_ptr)(cef_path_key_t, cef_string_t*);
typedef int (*cef_launch_process_ptr)(struct _cef_command_line_t*);
typedef int (*cef_register_scheme_handler_factory_ptr)(
    const cef_string_t*,
    const cef_string_t*,
    struct _cef_scheme_handler_factory_t*);
typedef int (*cef_clear_scheme_handler_factories_ptr)();
typedef int (*cef_is_cert_status_error_ptr)(cef_cert_status_t);
typedef int (*cef_currently_on_ptr)(cef_thread_id_t);
typedef int (*cef_post_task_ptr)(cef_thread_id_t, struct _cef_task_t*);
typedef int (*cef_post_delayed_task_ptr)(cef_thread_id_t,
                                         struct _cef_task_t*,
                                         int64);
typedef int (*cef_begin_tracing_ptr)(const cef_string_t*,
                                     struct _cef_completion_callback_t*);
typedef int (*cef_end_tracing_ptr)(const cef_string_t*,
                                   struct _cef_end_tracing_callback_t*);
typedef int64 (*cef_now_from_system_trace_time_ptr)();
typedef int (*cef_register_extension_ptr)(const cef_string_t*,
                                          const cef_string_t*,
                                          struct _cef_v8handler_t*);
typedef void (*cef_visit_web_plugin_info_ptr)(
    struct _cef_web_plugin_info_visitor_t*);
typedef void (*cef_refresh_web_plugins_ptr)();
typedef void (*cef_unregister_internal_web_plugin_ptr)(const cef_string_t*);
typedef void (*cef_register_web_plugin_crash_ptr)(const cef_string_t*);
typedef void (*cef_is_web_plugin_unstable_ptr)(
    const cef_string_t*,
    struct _cef_web_plugin_unstable_callback_t*);
typedef void (*cef_register_widevine_cdm_ptr)(
    const cef_string_t*,
    struct _cef_register_cdm_callback_t*);
typedef void (*cef_execute_java_script_with_user_gesture_for_tests_ptr)(
    struct _cef_frame_t*,
    const cef_string_t*);
typedef int (*cef_browser_host_create_browser_ptr)(
    const struct _cef_window_info_t*,
    struct _cef_client_t*,
    const cef_string_t*,
    const struct _cef_browser_settings_t*,
    struct _cef_dictionary_value_t*,
    struct _cef_request_context_t*);
typedef struct _cef_browser_t* (*cef_browser_host_create_browser_sync_ptr)(
    const struct _cef_window_info_t*,
    struct _cef_client_t*,
    const cef_string_t*,
    const struct _cef_browser_settings_t*,
    struct _cef_dictionary_value_t*,
    struct _cef_request_context_t*);
typedef struct _cef_command_line_t* (*cef_command_line_create_ptr)();
typedef struct _cef_command_line_t* (*cef_command_line_get_global_ptr)();
typedef struct _cef_cookie_manager_t* (
    *cef_cookie_manager_get_global_manager_ptr)(
    struct _cef_completion_callback_t*);
typedef struct _cef_drag_data_t* (*cef_drag_data_create_ptr)();
typedef struct _cef_image_t* (*cef_image_create_ptr)();
typedef struct _cef_media_router_t* (*cef_media_router_get_global_ptr)(
    struct _cef_completion_callback_t*);
typedef struct _cef_menu_model_t* (*cef_menu_model_create_ptr)(
    struct _cef_menu_model_delegate_t*);
typedef struct _cef_print_settings_t* (*cef_print_settings_create_ptr)();
typedef struct _cef_process_message_t* (*cef_process_message_create_ptr)(
    const cef_string_t*);
typedef struct _cef_request_t* (*cef_request_create_ptr)();
typedef struct _cef_post_data_t* (*cef_post_data_create_ptr)();
typedef struct _cef_post_data_element_t* (*cef_post_data_element_create_ptr)();
typedef struct _cef_request_context_t* (
    *cef_request_context_get_global_context_ptr)();
typedef struct _cef_request_context_t* (
    *cef_request_context_create_context_ptr)(
    const struct _cef_request_context_settings_t*,
    struct _cef_request_context_handler_t*);
typedef struct _cef_request_context_t* (*cef_create_context_shared_ptr)(
    struct _cef_request_context_t*,
    struct _cef_request_context_handler_t*);
typedef struct _cef_resource_bundle_t* (*cef_resource_bundle_get_global_ptr)();
typedef struct _cef_response_t* (*cef_response_create_ptr)();
typedef void (*cef_server_create_ptr)(const cef_string_t*,
                                      uint16,
                                      int,
                                      struct _cef_server_handler_t*);
typedef struct _cef_stream_reader_t* (*cef_stream_reader_create_for_file_ptr)(
    const cef_string_t*);
typedef struct _cef_stream_reader_t* (
    *cef_stream_reader_create_for_data_ptr)(void*, size_t);
typedef struct _cef_stream_reader_t* (
    *cef_stream_reader_create_for_handler_ptr)(struct _cef_read_handler_t*);
typedef struct _cef_stream_writer_t* (*cef_stream_writer_create_for_file_ptr)(
    const cef_string_t*);
typedef struct _cef_stream_writer_t* (
    *cef_stream_writer_create_for_handler_ptr)(struct _cef_write_handler_t*);
typedef struct _cef_task_runner_t* (
    *cef_task_runner_get_for_current_thread_ptr)();
typedef struct _cef_task_runner_t* (*cef_task_runner_get_for_thread_ptr)(
    cef_thread_id_t);
typedef struct _cef_thread_t* (*cef_thread_create_ptr)(const cef_string_t*,
                                                       cef_thread_priority_t,
                                                       cef_message_loop_type_t,
                                                       int,
                                                       cef_com_init_mode_t);
typedef struct _cef_urlrequest_t* (*cef_urlrequest_create_ptr)(
    struct _cef_request_t*,
    struct _cef_urlrequest_client_t*,
    struct _cef_request_context_t*);
typedef struct _cef_v8context_t* (*cef_v8context_get_current_context_ptr)();
typedef struct _cef_v8context_t* (*cef_v8context_get_entered_context_ptr)();
typedef int (*cef_v8context_in_context_ptr)();
typedef struct _cef_v8value_t* (*cef_v8value_create_undefined_ptr)();
typedef struct _cef_v8value_t* (*cef_v8value_create_null_ptr)();
typedef struct _cef_v8value_t* (*cef_v8value_create_bool_ptr)(int);
typedef struct _cef_v8value_t* (*cef_v8value_create_int_ptr)(int32);
typedef struct _cef_v8value_t* (*cef_v8value_create_uint_ptr)(uint32);
typedef struct _cef_v8value_t* (*cef_v8value_create_double_ptr)(double);
typedef struct _cef_v8value_t* (*cef_v8value_create_date_ptr)(
    const cef_time_t*);
typedef struct _cef_v8value_t* (*cef_v8value_create_string_ptr)(
    const cef_string_t*);
typedef struct _cef_v8value_t* (*cef_v8value_create_object_ptr)(
    struct _cef_v8accessor_t*,
    struct _cef_v8interceptor_t*);
typedef struct _cef_v8value_t* (*cef_v8value_create_array_ptr)(int);
typedef struct _cef_v8value_t* (*cef_v8value_create_array_buffer_ptr)(
    void*,
    size_t,
    struct _cef_v8array_buffer_release_callback_t*);
typedef struct _cef_v8value_t* (*cef_v8value_create_function_ptr)(
    const cef_string_t*,
    struct _cef_v8handler_t*);
typedef struct _cef_v8stack_trace_t* (*cef_v8stack_trace_get_current_ptr)(int);
typedef struct _cef_value_t* (*cef_value_create_ptr)();
typedef struct _cef_binary_value_t* (*cef_binary_value_create_ptr)(const void*,
                                                                   size_t);
typedef struct _cef_dictionary_value_t* (*cef_dictionary_value_create_ptr)();
typedef struct _cef_list_value_t* (*cef_list_value_create_ptr)();
typedef struct _cef_waitable_event_t* (*cef_waitable_event_create_ptr)(int,
                                                                       int);
typedef struct _cef_xml_reader_t* (*cef_xml_reader_create_ptr)(
    struct _cef_stream_reader_t*,
    cef_xml_encoding_type_t,
    const cef_string_t*);
typedef struct _cef_zip_reader_t* (*cef_zip_reader_create_ptr)(
    struct _cef_stream_reader_t*);
typedef struct _cef_translator_test_t* (*cef_translator_test_create_ptr)();
typedef struct _cef_translator_test_ref_ptr_library_t* (
    *cef_translator_test_ref_ptr_library_create_ptr)(int);
typedef struct _cef_translator_test_ref_ptr_library_child_t* (
    *cef_translator_test_ref_ptr_library_child_create_ptr)(int, int);
typedef struct _cef_translator_test_ref_ptr_library_child_child_t* (
    *cef_translator_test_ref_ptr_library_child_child_create_ptr)(int, int, int);
typedef struct _cef_translator_test_scoped_library_t* (
    *cef_translator_test_scoped_library_create_ptr)(int);
typedef struct _cef_translator_test_scoped_library_child_t* (
    *cef_translator_test_scoped_library_child_create_ptr)(int, int);
typedef struct _cef_translator_test_scoped_library_child_child_t* (
    *cef_translator_test_scoped_library_child_child_create_ptr)(int, int, int);
typedef struct _cef_browser_view_t* (*cef_browser_view_create_ptr)(
    struct _cef_client_t*,
    const cef_string_t*,
    const struct _cef_browser_settings_t*,
    struct _cef_dictionary_value_t*,
    struct _cef_request_context_t*,
    struct _cef_browser_view_delegate_t*);
typedef struct _cef_browser_view_t* (*cef_browser_view_get_for_browser_ptr)(
    struct _cef_browser_t*);
typedef struct _cef_display_t* (*cef_display_get_primary_ptr)();
typedef struct _cef_display_t* (
    *cef_display_get_nearest_point_ptr)(const cef_point_t*, int);
typedef struct _cef_display_t* (
    *cef_display_get_matching_bounds_ptr)(const cef_rect_t*, int);
typedef size_t (*cef_display_get_count_ptr)();
typedef void (*cef_display_get_alls_ptr)(size_t*, struct _cef_display_t**);
typedef struct _cef_label_button_t* (*cef_label_button_create_ptr)(
    struct _cef_button_delegate_t*,
    const cef_string_t*);
typedef struct _cef_menu_button_t* (*cef_menu_button_create_ptr)(
    struct _cef_menu_button_delegate_t*,
    const cef_string_t*);
typedef struct _cef_panel_t* (*cef_panel_create_ptr)(
    struct _cef_panel_delegate_t*);
typedef struct _cef_scroll_view_t* (*cef_scroll_view_create_ptr)(
    struct _cef_view_delegate_t*);
typedef struct _cef_textfield_t* (*cef_textfield_create_ptr)(
    struct _cef_textfield_delegate_t*);
typedef struct _cef_window_t* (*cef_window_create_top_level_ptr)(
    struct _cef_window_delegate_t*);
typedef const char* (*cef_api_hash_ptr)(int);
typedef int (*cef_version_info_ptr)(int);
typedef int (*cef_get_min_log_level_ptr)();
typedef int (*cef_get_vlog_level_ptr)(const char*, size_t);
typedef void (*cef_log_ptr)(const char*, int, int, const char*);
typedef cef_string_list_t (*cef_string_list_alloc_ptr)();
typedef size_t (*cef_string_list_size_ptr)(cef_string_list_t);
typedef int (*cef_string_list_value_ptr)(cef_string_list_t,
                                         size_t,
                                         cef_string_t*);
typedef void (*cef_string_list_append_ptr)(cef_string_list_t,
                                           const cef_string_t*);
typedef void (*cef_string_list_clear_ptr)(cef_string_list_t);
typedef void (*cef_string_list_free_ptr)(cef_string_list_t);
typedef cef_string_list_t (*cef_string_list_copy_ptr)(cef_string_list_t);
typedef cef_string_map_t (*cef_string_map_alloc_ptr)();
typedef size_t (*cef_string_map_size_ptr)(cef_string_map_t);
typedef int (*cef_string_map_find_ptr)(cef_string_map_t,
                                       const cef_string_t*,
                                       cef_string_t*);
typedef int (*cef_string_map_key_ptr)(cef_string_map_t, size_t, cef_string_t*);
typedef int (*cef_string_map_value_ptr)(cef_string_map_t,
                                        size_t,
                                        cef_string_t*);
typedef int (*cef_string_map_append_ptr)(cef_string_map_t,
                                         const cef_string_t*,
                                         const cef_string_t*);
typedef void (*cef_string_map_clear_ptr)(cef_string_map_t);
typedef void (*cef_string_map_free_ptr)(cef_string_map_t);
typedef cef_string_multimap_t (*cef_string_multimap_alloc_ptr)();
typedef size_t (*cef_string_multimap_size_ptr)(cef_string_multimap_t);
typedef size_t (*cef_string_multimap_find_count_ptr)(cef_string_multimap_t,
                                                     const cef_string_t*);
typedef int (*cef_string_multimap_enumerate_ptr)(cef_string_multimap_t,
                                                 const cef_string_t*,
                                                 size_t,
                                                 cef_string_t*);
typedef int (*cef_string_multimap_key_ptr)(cef_string_multimap_t,
                                           size_t,
                                           cef_string_t*);
typedef int (*cef_string_multimap_value_ptr)(cef_string_multimap_t,
                                             size_t,
                                             cef_string_t*);
typedef int (*cef_string_multimap_append_ptr)(cef_string_multimap_t,
                                              const cef_string_t*,
                                              const cef_string_t*);
typedef void (*cef_string_multimap_clear_ptr)(cef_string_multimap_t);
typedef void (*cef_string_multimap_free_ptr)(cef_string_multimap_t);
typedef int (*cef_string_wide_set_ptr)(const wchar_t*,
                                       size_t,
                                       cef_string_wide_t*,
                                       int);
typedef int (*cef_string_utf8_set_ptr)(const char*,
                                       size_t,
                                       cef_string_utf8_t*,
                                       int);
typedef int (*cef_string_utf16_set_ptr)(const char16*,
                                        size_t,
                                        cef_string_utf16_t*,
                                        int);
typedef void (*cef_string_wide_clear_ptr)(cef_string_wide_t*);
typedef void (*cef_string_utf8_clear_ptr)(cef_string_utf8_t*);
typedef void (*cef_string_utf16_clear_ptr)(cef_string_utf16_t*);
typedef int (*cef_string_wide_cmp_ptr)(const cef_string_wide_t*,
                                       const cef_string_wide_t*);
typedef int (*cef_string_utf8_cmp_ptr)(const cef_string_utf8_t*,
                                       const cef_string_utf8_t*);
typedef int (*cef_string_utf16_cmp_ptr)(const cef_string_utf16_t*,
                                        const cef_string_utf16_t*);
typedef int (*cef_string_wide_to_utf8_ptr)(const wchar_t*,
                                           size_t,
                                           cef_string_utf8_t*);
typedef int (*cef_string_utf8_to_wide_ptr)(const char*,
                                           size_t,
                                           cef_string_wide_t*);
typedef int (*cef_string_wide_to_utf16_ptr)(const wchar_t*,
                                            size_t,
                                            cef_string_utf16_t*);
typedef int (*cef_string_utf16_to_wide_ptr)(const char16*,
                                            size_t,
                                            cef_string_wide_t*);
typedef int (*cef_string_utf8_to_utf16_ptr)(const char*,
                                            size_t,
                                            cef_string_utf16_t*);
typedef int (*cef_string_utf16_to_utf8_ptr)(const char16*,
                                            size_t,
                                            cef_string_utf8_t*);
typedef int (*cef_string_ascii_to_wide_ptr)(const char*,
                                            size_t,
                                            cef_string_wide_t*);
typedef int (*cef_string_ascii_to_utf16_ptr)(const char*,
                                             size_t,
                                             cef_string_utf16_t*);
typedef cef_string_userfree_wide_t (*cef_string_userfree_wide_alloc_ptr)();
typedef cef_string_userfree_utf8_t (*cef_string_userfree_utf8_alloc_ptr)();
typedef cef_string_userfree_utf16_t (*cef_string_userfree_utf16_alloc_ptr)();
typedef void (*cef_string_userfree_wide_free_ptr)(cef_string_userfree_wide_t);
typedef void (*cef_string_userfree_utf8_free_ptr)(cef_string_userfree_utf8_t);
typedef void (*cef_string_userfree_utf16_free_ptr)(cef_string_userfree_utf16_t);
typedef int (*cef_string_utf16_to_lower_ptr)(const char16*,
                                             size_t,
                                             cef_string_utf16_t*);
typedef int (*cef_string_utf16_to_upper_ptr)(const char16*,
                                             size_t,
                                             cef_string_utf16_t*);
typedef cef_platform_thread_id_t (*cef_get_current_platform_thread_id_ptr)();
typedef cef_platform_thread_handle_t (
    *cef_get_current_platform_thread_handle_ptr)();
typedef int (*cef_time_to_timet_ptr)(const cef_time_t*, time_t*);
typedef int (*cef_time_from_timet_ptr)(time_t, cef_time_t*);
typedef int (*cef_time_to_doublet_ptr)(const cef_time_t*, double*);
typedef int (*cef_time_from_doublet_ptr)(double, cef_time_t*);
typedef int (*cef_time_now_ptr)(cef_time_t*);
typedef int (*cef_time_delta_ptr)(const cef_time_t*,
                                  const cef_time_t*,
                                  long long*);
typedef void (*cef_trace_event_instant_ptr)(const char*,
                                            const char*,
                                            const char*,
                                            uint64,
                                            const char*,
                                            uint64,
                                            int);
typedef void (*cef_trace_event_begin_ptr)(const char*,
                                          const char*,
                                          const char*,
                                          uint64,
                                          const char*,
                                          uint64,
                                          int);
typedef void (*cef_trace_event_end_ptr)(const char*,
                                        const char*,
                                        const char*,
                                        uint64,
                                        const char*,
                                        uint64,
                                        int);
typedef void (*cef_trace_counter_ptr)(const char*,
                                      const char*,
                                      const char*,
                                      uint64,
                                      const char*,
                                      uint64,
                                      int);
typedef void (*cef_trace_counter_id_ptr)(const char*,
                                         const char*,
                                         uint64,
                                         const char*,
                                         uint64,
                                         const char*,
                                         uint64,
                                         int);
typedef void (*cef_trace_event_async_begin_ptr)(const char*,
                                                const char*,
                                                uint64,
                                                const char*,
                                                uint64,
                                                const char*,
                                                uint64,
                                                int);
typedef void (*cef_trace_event_async_step_into_ptr)(const char*,
                                                    const char*,
                                                    uint64,
                                                    uint64,
                                                    const char*,
                                                    uint64,
                                                    int);
typedef void (*cef_trace_event_async_step_past_ptr)(const char*,
                                                    const char*,
                                                    uint64,
                                                    uint64,
                                                    const char*,
                                                    uint64,
                                                    int);
typedef void (*cef_trace_event_async_end_ptr)(const char*,
                                              const char*,
                                              uint64,
                                              const char*,
                                              uint64,
                                              const char*,
                                              uint64,
                                              int);


struct libcef_pointers2 {

  cef_execute_process_ptr cef_execute_process;
  cef_initialize_ptr cef_initialize;
  cef_shutdown_ptr cef_shutdown;
  cef_do_message_loop_work_ptr cef_do_message_loop_work;
  cef_run_message_loop_ptr cef_run_message_loop;
  cef_quit_message_loop_ptr cef_quit_message_loop;
  cef_set_osmodal_loop_ptr cef_set_osmodal_loop;
  cef_enable_highdpi_support_ptr cef_enable_highdpi_support;
  cef_crash_reporting_enabled_ptr cef_crash_reporting_enabled;
  cef_set_crash_key_value_ptr cef_set_crash_key_value;
     
  cef_create_directory_ptr cef_create_directory;
  cef_get_temp_directory_ptr cef_get_temp_directory;
  cef_create_new_temp_directory_ptr cef_create_new_temp_directory;
  cef_create_temp_directory_in_directory_ptr
      cef_create_temp_directory_in_directory;
  cef_directory_exists_ptr cef_directory_exists;
  cef_delete_file_ptr cef_delete_file;
  cef_zip_directory_ptr cef_zip_directory;
  cef_load_crlsets_file_ptr cef_load_crlsets_file;
  cef_add_cross_origin_whitelist_entry_ptr cef_add_cross_origin_whitelist_entry;
  cef_remove_cross_origin_whitelist_entry_ptr
      cef_remove_cross_origin_whitelist_entry;
  cef_clear_cross_origin_whitelist_ptr cef_clear_cross_origin_whitelist;
  cef_parse_url_ptr cef_parse_url;
  cef_create_url_ptr cef_create_url;
  cef_format_url_for_security_display_ptr cef_format_url_for_security_display;
  cef_get_mime_type_ptr cef_get_mime_type;
  cef_get_extensions_for_mime_type_ptr cef_get_extensions_for_mime_type;
  cef_base64encode_ptr cef_base64encode;
  cef_base64decode_ptr cef_base64decode;
  cef_uriencode_ptr cef_uriencode;
  cef_uridecode_ptr cef_uridecode;
  cef_parse_json_ptr cef_parse_json;
  cef_parse_json_buffer_ptr cef_parse_json_buffer;
  cef_parse_jsonand_return_error_ptr cef_parse_jsonand_return_error;
  cef_write_json_ptr cef_write_json;
  cef_get_path_ptr cef_get_path;
  cef_launch_process_ptr cef_launch_process;
  cef_register_scheme_handler_factory_ptr cef_register_scheme_handler_factory;
  cef_clear_scheme_handler_factories_ptr cef_clear_scheme_handler_factories;
  cef_is_cert_status_error_ptr cef_is_cert_status_error;
  cef_currently_on_ptr cef_currently_on;
  cef_post_task_ptr cef_post_task;
  cef_post_delayed_task_ptr cef_post_delayed_task;
  cef_begin_tracing_ptr cef_begin_tracing;
  cef_end_tracing_ptr cef_end_tracing;
  cef_now_from_system_trace_time_ptr cef_now_from_system_trace_time;
  cef_register_extension_ptr cef_register_extension;
  cef_visit_web_plugin_info_ptr cef_visit_web_plugin_info;
  cef_refresh_web_plugins_ptr cef_refresh_web_plugins;
  cef_unregister_internal_web_plugin_ptr cef_unregister_internal_web_plugin;
  cef_register_web_plugin_crash_ptr cef_register_web_plugin_crash;
  cef_is_web_plugin_unstable_ptr cef_is_web_plugin_unstable;
  cef_register_widevine_cdm_ptr cef_register_widevine_cdm;
  cef_execute_java_script_with_user_gesture_for_tests_ptr
      cef_execute_java_script_with_user_gesture_for_tests;
  cef_browser_host_create_browser_ptr cef_browser_host_create_browser;
  cef_browser_host_create_browser_sync_ptr cef_browser_host_create_browser_sync;
  cef_command_line_create_ptr cef_command_line_create;
  cef_command_line_get_global_ptr cef_command_line_get_global;
  cef_cookie_manager_get_global_manager_ptr
      cef_cookie_manager_get_global_manager;
  cef_drag_data_create_ptr cef_drag_data_create;
  cef_image_create_ptr cef_image_create;
  cef_media_router_get_global_ptr cef_media_router_get_global;
  cef_menu_model_create_ptr cef_menu_model_create;
  cef_print_settings_create_ptr cef_print_settings_create;
  cef_process_message_create_ptr cef_process_message_create;
  cef_request_create_ptr cef_request_create;
  cef_post_data_create_ptr cef_post_data_create;
  cef_post_data_element_create_ptr cef_post_data_element_create;
  cef_request_context_get_global_context_ptr
      cef_request_context_get_global_context;
  cef_request_context_create_context_ptr cef_request_context_create_context;
  cef_create_context_shared_ptr cef_create_context_shared;
  cef_resource_bundle_get_global_ptr cef_resource_bundle_get_global;
  cef_response_create_ptr cef_response_create;
  cef_server_create_ptr cef_server_create;
  cef_stream_reader_create_for_file_ptr cef_stream_reader_create_for_file;
  cef_stream_reader_create_for_data_ptr cef_stream_reader_create_for_data;
  cef_stream_reader_create_for_handler_ptr cef_stream_reader_create_for_handler;
  cef_stream_writer_create_for_file_ptr cef_stream_writer_create_for_file;
  cef_stream_writer_create_for_handler_ptr cef_stream_writer_create_for_handler;
  cef_task_runner_get_for_current_thread_ptr
      cef_task_runner_get_for_current_thread;
  cef_task_runner_get_for_thread_ptr cef_task_runner_get_for_thread;
  cef_thread_create_ptr cef_thread_create;
  cef_urlrequest_create_ptr cef_urlrequest_create;
  cef_v8context_get_current_context_ptr cef_v8context_get_current_context;
  cef_v8context_get_entered_context_ptr cef_v8context_get_entered_context;
  cef_v8context_in_context_ptr cef_v8context_in_context;
  cef_v8value_create_undefined_ptr cef_v8value_create_undefined;
  cef_v8value_create_null_ptr cef_v8value_create_null;
  cef_v8value_create_bool_ptr cef_v8value_create_bool;
  cef_v8value_create_int_ptr cef_v8value_create_int;
  cef_v8value_create_uint_ptr cef_v8value_create_uint;
  cef_v8value_create_double_ptr cef_v8value_create_double;
  cef_v8value_create_date_ptr cef_v8value_create_date;
  cef_v8value_create_string_ptr cef_v8value_create_string;
  cef_v8value_create_object_ptr cef_v8value_create_object;
  cef_v8value_create_array_ptr cef_v8value_create_array;
  cef_v8value_create_array_buffer_ptr cef_v8value_create_array_buffer;
  cef_v8value_create_function_ptr cef_v8value_create_function;
  cef_v8stack_trace_get_current_ptr cef_v8stack_trace_get_current;
  cef_value_create_ptr cef_value_create;
  cef_binary_value_create_ptr cef_binary_value_create;
  cef_dictionary_value_create_ptr cef_dictionary_value_create;
  cef_list_value_create_ptr cef_list_value_create;
  cef_waitable_event_create_ptr cef_waitable_event_create;
  cef_xml_reader_create_ptr cef_xml_reader_create;
  cef_zip_reader_create_ptr cef_zip_reader_create;
  cef_translator_test_create_ptr cef_translator_test_create;
  cef_translator_test_ref_ptr_library_create_ptr
      cef_translator_test_ref_ptr_library_create;
  cef_translator_test_ref_ptr_library_child_create_ptr
      cef_translator_test_ref_ptr_library_child_create;
  cef_translator_test_ref_ptr_library_child_child_create_ptr
      cef_translator_test_ref_ptr_library_child_child_create;
  cef_translator_test_scoped_library_create_ptr
      cef_translator_test_scoped_library_create;
  cef_translator_test_scoped_library_child_create_ptr
      cef_translator_test_scoped_library_child_create;
  cef_translator_test_scoped_library_child_child_create_ptr
      cef_translator_test_scoped_library_child_child_create;
  cef_browser_view_create_ptr cef_browser_view_create;
  cef_browser_view_get_for_browser_ptr cef_browser_view_get_for_browser;
  cef_display_get_primary_ptr cef_display_get_primary;
  cef_display_get_nearest_point_ptr cef_display_get_nearest_point;
  cef_display_get_matching_bounds_ptr cef_display_get_matching_bounds;
  cef_display_get_count_ptr cef_display_get_count;
  cef_display_get_alls_ptr cef_display_get_alls;
  cef_label_button_create_ptr cef_label_button_create;
  cef_menu_button_create_ptr cef_menu_button_create;
  cef_panel_create_ptr cef_panel_create;
  cef_scroll_view_create_ptr cef_scroll_view_create;
  cef_textfield_create_ptr cef_textfield_create;
  cef_window_create_top_level_ptr cef_window_create_top_level;
  cef_api_hash_ptr cef_api_hash;
  cef_version_info_ptr cef_version_info;
  cef_get_min_log_level_ptr cef_get_min_log_level;
  cef_get_vlog_level_ptr cef_get_vlog_level;
  cef_log_ptr cef_log;
  cef_string_list_alloc_ptr cef_string_list_alloc;
  cef_string_list_size_ptr cef_string_list_size;
  cef_string_list_value_ptr cef_string_list_value;
  cef_string_list_append_ptr cef_string_list_append;
  cef_string_list_clear_ptr cef_string_list_clear;
  cef_string_list_free_ptr cef_string_list_free;
  cef_string_list_copy_ptr cef_string_list_copy;
  cef_string_map_alloc_ptr cef_string_map_alloc;
  cef_string_map_size_ptr cef_string_map_size;
  cef_string_map_find_ptr cef_string_map_find;
  cef_string_map_key_ptr cef_string_map_key;
  cef_string_map_value_ptr cef_string_map_value;
  cef_string_map_append_ptr cef_string_map_append;
  cef_string_map_clear_ptr cef_string_map_clear;
  cef_string_map_free_ptr cef_string_map_free;
  cef_string_multimap_alloc_ptr cef_string_multimap_alloc;
  cef_string_multimap_size_ptr cef_string_multimap_size;
  cef_string_multimap_find_count_ptr cef_string_multimap_find_count;
  cef_string_multimap_enumerate_ptr cef_string_multimap_enumerate;
  cef_string_multimap_key_ptr cef_string_multimap_key;
  cef_string_multimap_value_ptr cef_string_multimap_value;
  cef_string_multimap_append_ptr cef_string_multimap_append;
  cef_string_multimap_clear_ptr cef_string_multimap_clear;
  cef_string_multimap_free_ptr cef_string_multimap_free;
  cef_string_wide_set_ptr cef_string_wide_set;
  cef_string_utf8_set_ptr cef_string_utf8_set;
  cef_string_utf16_set_ptr cef_string_utf16_set;
  cef_string_wide_clear_ptr cef_string_wide_clear;
  cef_string_utf8_clear_ptr cef_string_utf8_clear;
  cef_string_utf16_clear_ptr cef_string_utf16_clear;
  cef_string_wide_cmp_ptr cef_string_wide_cmp;
  cef_string_utf8_cmp_ptr cef_string_utf8_cmp;
  cef_string_utf16_cmp_ptr cef_string_utf16_cmp;
  cef_string_wide_to_utf8_ptr cef_string_wide_to_utf8;
  cef_string_utf8_to_wide_ptr cef_string_utf8_to_wide;
  cef_string_wide_to_utf16_ptr cef_string_wide_to_utf16;
  cef_string_utf16_to_wide_ptr cef_string_utf16_to_wide;
  cef_string_utf8_to_utf16_ptr cef_string_utf8_to_utf16;
  cef_string_utf16_to_utf8_ptr cef_string_utf16_to_utf8;
  cef_string_ascii_to_wide_ptr cef_string_ascii_to_wide;
  cef_string_ascii_to_utf16_ptr cef_string_ascii_to_utf16;
  cef_string_userfree_wide_alloc_ptr cef_string_userfree_wide_alloc;
  cef_string_userfree_utf8_alloc_ptr cef_string_userfree_utf8_alloc;
  cef_string_userfree_utf16_alloc_ptr cef_string_userfree_utf16_alloc;
  cef_string_userfree_wide_free_ptr cef_string_userfree_wide_free;
  cef_string_userfree_utf8_free_ptr cef_string_userfree_utf8_free;
  cef_string_userfree_utf16_free_ptr cef_string_userfree_utf16_free;
  cef_string_utf16_to_lower_ptr cef_string_utf16_to_lower;
  cef_string_utf16_to_upper_ptr cef_string_utf16_to_upper;
  cef_get_current_platform_thread_id_ptr cef_get_current_platform_thread_id;
  cef_get_current_platform_thread_handle_ptr
      cef_get_current_platform_thread_handle;
  cef_time_to_timet_ptr cef_time_to_timet;
  cef_time_from_timet_ptr cef_time_from_timet;
  cef_time_to_doublet_ptr cef_time_to_doublet;
  cef_time_from_doublet_ptr cef_time_from_doublet;
  cef_time_now_ptr cef_time_now;
  cef_time_delta_ptr cef_time_delta;
  cef_trace_event_instant_ptr cef_trace_event_instant;
  cef_trace_event_begin_ptr cef_trace_event_begin;
  cef_trace_event_end_ptr cef_trace_event_end;
  cef_trace_counter_ptr cef_trace_counter;
  cef_trace_counter_id_ptr cef_trace_counter_id;
  cef_trace_event_async_begin_ptr cef_trace_event_async_begin;
  cef_trace_event_async_step_into_ptr cef_trace_event_async_step_into;
  cef_trace_event_async_step_past_ptr cef_trace_event_async_step_past;
  cef_trace_event_async_end_ptr cef_trace_event_async_end;

} g_libcef_pointers2 = {0};

#ifdef _WIN32
#elif defined(__APPLE__)
#else
namespace {
int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  LOG(WARNING) << "X error received: "
               << "type " << event->type << ", "
               << "serial " << event->serial << ", "
               << "error_code " << static_cast<int>(event->error_code) << ", "
               << "request_code " << static_cast<int>(event->request_code)
               << ", "
               << "minor_code " << static_cast<int>(event->minor_code);
  return 0;
}

int XIOErrorHandlerImpl(Display *display) { return 0; }
}
#endif

#ifdef _WIN32
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  // Do all the standard CEF setup stuff.
  //-------------------------------------

  void *sandbox_info = nullptr;

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  // Read command line arguments.
  CefMainArgs main_args(hInstance);
  const auto cliOptions = loot::CommandLineOptions();

  // Create the process reference.
  CefRefPtr<loot::LootApp> app(new loot::LootApp(cliOptions));

  // Run the process.
  int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Check if LOOT is already running
  //---------------------------------

  HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"LOOT.Shell.Instance");
  if (hMutex != NULL) {
    // An instance of LOOT is already running, so focus its window then quit.
    HWND hWnd = ::FindWindow(NULL, L"LOOT");
    ::SetForegroundWindow(hWnd);
    return 0;
  } else {
    // Create the mutex so that future instances will not run.
    hMutex = ::CreateMutex(NULL, FALSE, L"LOOT.Shell.Instance");
  }

  // Back to CEF
  //------------

  // Initialise CEF settings.
  CefSettings cef_settings = GetCefSettings(app.get()->getL10nPath());

  // Initialize CEF.
  CefInitialize(main_args, cef_settings, app.get(), sandbox_info);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  // Release the program instance mutex.
  if (hMutex != NULL) {
    ReleaseMutex(hMutex);
  }

  return 0;
}

#elif defined(__APPLE__)
#include <cef/include/cef_sandbox_mac.h>
int main(int argc, char *argv[]) {
  // Do all the standard CEF setup stuff.
  //-------------------------------------

  void *sandbox_info = nullptr;

    // see Framework linking and loading changes on macOS starting with 3538 branch (https://groups.google.com/g/cef-announce/c/Fith0A3kWtw)
      //CefScopedSandboxContext sandbox_context;
      //if (!sandbox_context.Initialize(argc, argv))
         // return 1;
        
      // Load the CEF framework library at runtime instead of linking directly
      // as required by the macOS sandbox implementation.
      CefScopedLibraryLoader library_loader;
      if (!library_loader.LoadInHelper())
        return 1;
    
  cef_api_hash_ptr a = g_libcef_pointers2.cef_api_hash;
  // Read command line arguments.
  CefMainArgs main_args(argc, argv);
  const auto cliOptions = loot::CommandLineOptions(argc, argv);

  // Create the process reference.
  CefRefPtr<loot::LootApp> app(new loot::LootApp(cliOptions));

  // Run the process.
  int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Initialise CEF settings.
  CefSettings cef_settings = GetCefSettings(app.get()->getL10nPath());
    //std::string a1 = app.get()->getL10nPath();
    //CefSettings cef_settings = GetCefSettings("/Volumes/MacintoshHD-資料/Dropbox/Project/Chromium Embedded Framework.framework/Resources");
    //CefSettings cef_settings;
  // Initialize CEF.
  CefInitialize(main_args, cef_settings, app.get(),  NULL);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  return 0;
}
#else
int main(int argc, char *argv[]) {
  // Do all the standard CEF setup stuff.
  //-------------------------------------

  void *sandbox_info = nullptr;

  // Read command line arguments.
  CefMainArgs main_args(argc, argv);
  const auto cliOptions = loot::CommandLineOptions(argc, argv);

  // Create the process reference.
  CefRefPtr<loot::LootApp> app(new loot::LootApp(cliOptions));

  // Run the process.
  int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Initialise CEF settings.
  CefSettings cef_settings = GetCefSettings(app.get()->getL10nPath());

  // Install xlib error handlers so that the application won't be terminated
  // on non-fatal errors.
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);

  // Initialize CEF.
  CefInitialize(main_args, cef_settings, app.get(), sandbox_info);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  return 0;
}
#endif
*/
