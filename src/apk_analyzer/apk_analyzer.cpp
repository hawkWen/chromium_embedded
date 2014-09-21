#include "stdafx.h"

#include <new>
#include <windows.h>

#include "base/at_exit.h"
#include "include/cef_sandbox_win.h"
#include "apk_analyzer_app.h"

// Set to 0 to disable sandbox support
#define CEF_ENABLE_SANDBOX 0

#if CEF_ENABLE_SANDBOX
#pragma comment(lib, "cef_sandbox.lib")
#endif

int APIENTRY wWinMain(
  _In_ HINSTANCE instance,
  _In_opt_ HINSTANCE prev_instance,
  _In_ LPWSTR cmd_line,
  _In_ int show_cmd) {

  base::AtExitManager exit_manager;
  
  UNREFERENCED_PARAMETER(prev_instance);
  UNREFERENCED_PARAMETER(cmd_line);

  void* sandbox_info = NULL;

#if CEF_ENABLE_SANDBOX
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif

  CefMainArgs main_args(instance);

  CefRefPtr<apk_analyzer::ApkAnalyzerApp> app = new (std::nothrow)apk_analyzer::ApkAnalyzerApp();

  int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
  if (0 <= exit_code) {
    return exit_code;
  }

  CefSettings settings;

#if !CEF_ENABLE_SANDBOX
  settings.no_sandbox = true;
#endif

  // Initialize CEF
  CefInitialize(main_args, settings, app.get(), sandbox_info);

  CefRunMessageLoop();

  CefShutdown();

  return 0;
}