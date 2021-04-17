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
// $hash=6be8fd993c46effa1b6c0a953c253d478a14c302$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_V8HANDLER_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_V8HANDLER_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_v8_capi.h"
#include "include/cef_v8.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefV8HandlerCppToC : public CefCppToCRefCounted<CefV8HandlerCppToC,
                                                      CefV8Handler,
                                                      cef_v8handler_t> {
 public:
  CefV8HandlerCppToC();
  virtual ~CefV8HandlerCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_V8HANDLER_CPPTOC_H_
