#include "apk_analyzer_handler.h"

#include <sstream>

#include "include/cef_app.h"
#include "include/cef_runnable.h"
#include "include/cef_frame.h"
#include "include/cef_task.h"
#include "include/internal/cef_types.h"
#include "util.h"

namespace apk_analyzer {

ApkAnalyzerHandler::ApkAnalyzerHandler()
 : is_closeing_(false) {
}

ApkAnalyzerHandler::~ApkAnalyzerHandler() {
}

CefRefPtr<CefDisplayHandler> ApkAnalyzerHandler::GetDisplayHandler() {
  return this;
}

CefRefPtr<CefLifeSpanHandler> ApkAnalyzerHandler::GetLifeSpanHandler() {
  return this;
}

CefRefPtr<CefLoadHandler> ApkAnalyzerHandler::GetLoadHandler() {
  return this;
}

CefRefPtr<CefRequestHandler> ApkAnalyzerHandler::GetRequestHandler() {
  return this;
}

CefRefPtr<CefContextMenuHandler> ApkAnalyzerHandler::GetContextMenuHandler() {
  return this;
}

CefRefPtr<CefRenderHandler> ApkAnalyzerHandler::GetRenderHandler() {
  return this;
}

//////////////////////////////////////////////////////////////////////////
// CefClient
bool ApkAnalyzerHandler::OnProcessMessageReceived(
  CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {

  return true;
  REQUIRE_UI_THREAD();
  if (message_router_->OnProcessMessageReceived(browser, source_process, message)) {
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
void ApkAnalyzerHandler::OnTitleChange(
  CefRefPtr<CefBrowser> browser,
  const CefString& title) {
}

void ApkAnalyzerHandler::OnAfterCreated(
  CefRefPtr<CefBrowser> browser) {

  REQUIRE_UI_THREAD();

  _InitializeOnAfterCreated();
  
  browser_list_.push_back(browser);
}

void ApkAnalyzerHandler::OnBeforeClose(
  CefRefPtr<CefBrowser> browser) {

  REQUIRE_UI_THREAD();

  message_router_->OnBeforeClose(browser);

  for (BrowserList::iterator iter = browser_list_.begin();
    iter != browser_list_.end(); ++iter) {

    if ((*iter)->IsSame(browser)) {
      browser_list_.erase(iter);
      break;
    }
  }

  if (browser_list_.empty()) {
    _UninitializeOnBeforeClose(browser);
  }
}

bool ApkAnalyzerHandler::DoClose(
  CefRefPtr<CefBrowser> browser) {

  REQUIRE_UI_THREAD();

  if (1 == browser_list_.size()) {
    is_closeing_ = true;
  }

  return false;
}

void ApkAnalyzerHandler::OnLoadError(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  ErrorCode error_code,
  const CefString& error_text,
  const CefString& failed_url) {
  
  REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files
  if (ERR_ABORTED == error_code) {
    return;
  }

  // Display a load error message
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
    "<h2>Failed to load URL " << std::string(failed_url) <<
    " with error " << std::string(error_text) << " (" << error_code <<
    ").</h2></body></html>";

  frame->LoadString(ss.str(), failed_url);
}

//////////////////////////////////////////////////////////////////////////
bool ApkAnalyzerHandler::OnBeforeBrowse(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefRequest> request,
  bool is_redirect) {

  message_router_->OnBeforeBrowse(browser, frame);
  return false;
}

void ApkAnalyzerHandler::OnRenderProcessTerminated(
  CefRefPtr<CefBrowser> browser,
  TerminationStatus status) {

  REQUIRE_UI_THREAD();

  message_router_->OnRenderProcessTerminated(browser);
}


//////////////////////////////////////////////////////////////////////////
//
void ApkAnalyzerHandler::OnBeforeContextMenu(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefContextMenuParams> params,
  CefRefPtr<CefMenuModel> model) {
}


//////////////////////////////////////////////////////////////////////////
//
bool ApkAnalyzerHandler::GetRootScreenRect(
  CefRefPtr<CefBrowser> browser,
  CefRect& rect) {
  return false;
}

bool ApkAnalyzerHandler::GetViewRect(
  CefRefPtr<CefBrowser> browser,
  CefRect& rect) {
  return false;
}

bool ApkAnalyzerHandler::GetScreenPoint(
  CefRefPtr<CefBrowser> browser,
  int view_x,
  int view_y,
  int& screen_x,
  int& screen_y) {
  return false;
}

bool ApkAnalyzerHandler::GetScreenInfo(
  CefRefPtr<CefBrowser> browser,
  CefScreenInfo& screen_info) {
  return false;
}

void ApkAnalyzerHandler::OnPopupSize(
  CefRefPtr<CefBrowser> browser,
  const CefRect& rect) {
}

void ApkAnalyzerHandler::OnPaint(
  CefRefPtr<CefBrowser> browser,
  PaintElementType type,
  const RectList& dirty_rects,
  const void* buffer,
  int width,
  int height) {
}

void ApkAnalyzerHandler::OnCursorChange(
  CefRefPtr<CefBrowser> browser,
  CefCursorHandle cursor) {
}

bool ApkAnalyzerHandler::StartDragging(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefDragData> drag_data,
  DragOperationsMask allowed_ops,
  int x,
  int y) {
  return false;
}

void ApkAnalyzerHandler::UpdateDragCursor(
  CefRefPtr<CefBrowser> browser,
  DragOperation operation) {
}

void ApkAnalyzerHandler::OnScrollOffsetChanged(
  CefRefPtr<CefBrowser> browser) {
}


//////////////////////////////////////////////////////////////////////////
void ApkAnalyzerHandler::CloseAllBrowsers(const
  bool force_close) {
  
  CefCurrentlyOn(TID_UI);
  if (!CefCurrentlyOn(TID_UI)) {
    CefPostTask(TID_UI, NewCefRunnableMethod(this, & ApkAnalyzerHandler::CloseAllBrowsers, force_close));
    return;
  }

  if (browser_list_.empty()) {
    return;
  }

  for (BrowserList::const_iterator iter = browser_list_.begin();
    iter != browser_list_.end(); ++iter) {
    (*iter)->GetHost()->CloseBrowser(force_close);
  }
}

//////////////////////////////////////////////////////////////////////////

// will call many time
bool ApkAnalyzerHandler::_InitializeOnAfterCreated() {

  REQUIRE_UI_THREAD();

  if (nullptr == message_router_) {
    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterBrowserSide::Create(config);

    //
    _CreateMessageHandlers(message_handler_set_);
    for (MessageHandlerSet::const_iterator iter = message_handler_set_.begin();
      iter != message_handler_set_.end(); ++iter) {
      message_router_->AddHandler(*(iter), false);
    }
  }

  return true;
}

bool ApkAnalyzerHandler::_UninitializeOnBeforeClose(
  const CefRefPtr<CefBrowser>& browser) {

  REQUIRE_UI_THREAD();

  for (MessageHandlerSet::iterator iter = message_handler_set_.begin();
    iter != message_handler_set_.end(); ++iter) {
    message_router_->RemoveHandler((*iter));
    delete (*iter);
  }
  message_handler_set_.clear();
  message_router_ = nullptr;

  CefQuitMessageLoop();
  return true;
}

//
void ApkAnalyzerHandler::_CreateMessageHandlers(
  MessageHandlerSet& handlers) {
  
  REQUIRE_UI_THREAD();

}
}