#include "apk_analyzer_app.h"

#include <new>
#include <algorithm>

#include "util.h"
#include "include/common_function.h"
#include "apk_analyzer_handler.h"

#include "delegate/client_render_delegate.h"

namespace apk_analyzer {

static const std::string kMainPath = "www\\main.html";

ApkAnalyzerApp::ApkAnalyzerApp() {
}

ApkAnalyzerApp::~ApkAnalyzerApp() {
}


//////////////////////////////////////////////////////////////////////////
// CefApp
CefRefPtr<CefBrowserProcessHandler> ApkAnalyzerApp::GetBrowserProcessHandler() {
  return this;
}

CefRefPtr<CefRenderProcessHandler> ApkAnalyzerApp::GetRenderProcessHandler() {
  return this;
}


//////////////////////////////////////////////////////////////////////////
// CefBrowserProcessHandler
void ApkAnalyzerApp::OnContextInitialized() {
  
  REQUIRE_UI_THREAD();

  CefWindowInfo window_info;

  window_info.SetAsPopup(NULL, "apk_analyzer");

  CefRefPtr<ApkAnalyzerHandler> handler = new (std::nothrow)ApkAnalyzerHandler;

  CefBrowserSettings browser_settings;

  std::string url;

  CefRefPtr<CefCommandLine> command_line =
    CefCommandLine::GetGlobalCommandLine();
  url = command_line->GetSwitchValue("url");
  if (url.empty()) {
    GetProgramDirectoryPath(url);
    url.append(kMainPath);
  }
  
  //url = "d:\\Workspace\\cef_project\\bin\\debug\\www\\main.html";
  url = "http://www.baidu.com";
  CefBrowserHost::CreateBrowser(window_info, handler.get(), url, browser_settings, NULL);
}

void ApkAnalyzerApp::OnBeforeChildProcessLaunch(
  CefRefPtr<CefCommandLine> command_line) {
}

void ApkAnalyzerApp::OnRenderProcessThreadCreated(
  CefRefPtr<CefListValue> extra_info) {
}


//////////////////////////////////////////////////////////////////////////
// CefRenderProcessHandler

void ApkAnalyzerApp::OnRenderThreadCreated(
  CefRefPtr<CefListValue> extra_info) {

  _CreateRenderDelegates(render_delegates_);

  for (auto& val : render_delegates_) {
    val->OnRenderThreadCreated(this, extra_info);
  }
}

void ApkAnalyzerApp::OnWebKitInitialized() {

  for (auto& val : render_delegates_) {
    val->OnWebKitInitialized(this);
  }
}

void ApkAnalyzerApp::OnBrowserCreated(
  CefRefPtr<CefBrowser> browser) {

  for (auto& val : render_delegates_) {
    val->OnBrowserCreated(this, browser);
  }
}

void ApkAnalyzerApp::OnBrowserDestroyed(
  CefRefPtr<CefBrowser> browser) {

  for (auto& val : render_delegates_) {
    val->OnBrowserDestroyed(this, browser);
  }
}

CefRefPtr<CefLoadHandler> ApkAnalyzerApp::GetLoadHandler() {
  
  CefRefPtr<CefLoadHandler> load_handler;

  std::find_if(render_delegates_.begin(), render_delegates_.end(),
    [&](const CefRefPtr<RenderDelegate>& val)->bool {
    return (nullptr != (load_handler = val->GetLoadHandler(this)));
  });
  return load_handler;
}

bool ApkAnalyzerApp::OnBeforeNavigation(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefRequest> request,
  NavigationType navigation_type,
  bool is_redirect) {
  return false;
}

void ApkAnalyzerApp::OnContextCreated(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {

  for (auto& val : render_delegates_) {
    val->OnContextCreated(this, browser, frame, context);
  }
}

void ApkAnalyzerApp::OnContextReleased(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {
  
}

void ApkAnalyzerApp::OnUncaughtException(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context,
  CefRefPtr<CefV8Exception> exception,
  CefRefPtr<CefV8StackTrace> stack_trace) {
}

void ApkAnalyzerApp::OnFocusedNodeChanged(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefDOMNode> node) {
}

bool ApkAnalyzerApp::OnProcessMessageReceived(
  CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {

  bool is_handled = false;

  std::find_if(render_delegates_.begin(), render_delegates_.end(),
    [&](const CefRefPtr<RenderDelegate>& val)->bool {
    return (true == (is_handled = val->OnProcessMessageReceived(this, browser, source_process, message)));
  });

  return is_handled;
}

//////////////////////////////////////////////////////////////////////////
// inline function
inline void ApkAnalyzerApp::_CreateRenderDelegates(
  RenderDelegateSet& delegates) {
  
  ClientRenderDelegate::CreateRenderDelegate(delegates);
}
}