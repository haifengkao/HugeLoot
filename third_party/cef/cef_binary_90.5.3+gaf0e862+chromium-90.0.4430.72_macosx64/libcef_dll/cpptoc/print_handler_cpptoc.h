// Copyright (c) 2021 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=525179c2c2bf4727855e0fa8610fba5ec050b19d$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_PRINT_HANDLER_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_PRINT_HANDLER_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_print_handler_capi.h"
#include "include/cef_print_handler.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefPrintHandlerCppToC : public CefCppToCRefCounted<CefPrintHandlerCppToC,
                                                         CefPrintHandler,
                                                         cef_print_handler_t> {
 public:
  CefPrintHandlerCppToC();
  virtual ~CefPrintHandlerCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_PRINT_HANDLER_CPPTOC_H_
