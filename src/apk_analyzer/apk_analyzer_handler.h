#ifndef APK_ANALYZER_APK_ANALYZER_HANDLER_H_
#define APK_ANALYZER_APK_ANALYZER_HANDLER_H_

#include <list>
#include <set>

#include "include/cef_client.h"
#include "include/base/cef_lock.h"
#include "include/common_define.h"
#include "include/base/cef_build.h"
#include "include/wrapper/cef_message_router.h"

namespace apk_analyzer {

class ApkAnalyzerHandler : public CefClient,
                           public CefDisplayHandler,
                           public CefLifeSpanHandler,
                           public CefLoadHandler,
                           public CefRequestHandler,
                           public CefContextMenuHandler,
                           public CefRenderHandler {
private:
  enum ClientMenuIds {
    kClientIdShowDevTools = MENU_ID_USER_FIRST,
    kClientIdCloseDevTools,
  };

public:
  ApkAnalyzerHandler();

  //////////////////////////////////////////////////////////////////////////
  // CefClient method
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;

  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;

  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // CefClient
  virtual bool OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) OVERRIDE;

  //////////////////////////////////////////////////////////////////////////
  // CefDisplayHandler methods
  virtual void OnTitleChange(
    CefRefPtr<CefBrowser> browser,
    const CefString& title) OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // CefLifeSpanHandler methods
  virtual void OnAfterCreated(
    CefRefPtr<CefBrowser> browser) OVERRIDE;

  virtual void OnBeforeClose(
    CefRefPtr<CefBrowser> browser) OVERRIDE;

  virtual bool DoClose(
    CefRefPtr<CefBrowser> browser) OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // CefLoadHandler
  virtual void OnLoadError(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    ErrorCode error_code,
    const CefString& error_text,
    const CefString& failed_url) OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // CefRequestHandler method
  virtual bool OnBeforeBrowse(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool is_redirect) OVERRIDE;

  virtual void OnRenderProcessTerminated(
    CefRefPtr<CefBrowser> browser,
    TerminationStatus status) OVERRIDE;

  
  //////////////////////////////////////////////////////////////////////////
  // CefContextMenuHandler
  virtual void OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model);


  //////////////////////////////////////////////////////////////////////////
  // CefRenderHandler
  
  // Called to retrieve the root window rectangle in screen coordinates.
  // true if the rectangle was provided
  virtual bool GetRootScreenRect(
    CefRefPtr<CefBrowser> browser,
    CefRect& rect) OVERRIDE;

  // Called to retrieve the view rectangle which is relative to screen
  // coordinates. Return true if the rectangle was provided
  virtual bool GetViewRect(
    CefRefPtr<CefBrowser> browser,
    CefRect& rect) OVERRIDE;

  // Called to retrieve the translation from view coordinates to actual screen
  // coordinates. Return true if the screen coordinates were provided.
  virtual bool GetScreenPoint(
    CefRefPtr<CefBrowser> browser,
    int view_x,
    int view_y,
    int& screen_x,
    int& screen_y) OVERRIDE;

  // Called to allow the client to fill in the CefScreenInfo object with
  // appropriate value. Return true if the |screen_info| structure has been
  // modified
  virtual bool GetScreenInfo(
    CefRefPtr<CefBrowser> browser,
    CefScreenInfo& screen_info) OVERRIDE;

  // Called
  virtual void OnPopupSize(
    CefRefPtr<CefBrowser> browser,
    const CefRect& rect) OVERRIDE;

  // Called 
  virtual void OnPaint(
    CefRefPtr<CefBrowser> browser,
    PaintElementType type,
    const RectList& dirty_rects,
    const void* buffer,
    int width,
    int height) OVERRIDE;

  // Called
  virtual void OnCursorChange(
    CefRefPtr<CefBrowser> browser,
    CefCursorHandle cursor) OVERRIDE;

  // Called
  virtual bool StartDragging(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> drag_data,
    DragOperationsMask allowed_ops,
    int x,
    int y) OVERRIDE;

  // Called
  virtual void UpdateDragCursor(
    CefRefPtr<CefBrowser> browser,
    DragOperation operation) OVERRIDE;

  // Called
  virtual void OnScrollOffsetChanged(
    CefRefPtr<CefBrowser> browser) OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // Request that all existing browser window close
  void CloseAllBrowsers(
    const bool force_close);


  inline bool IsClosing() const {
    return is_closeing_;
  }

private:
  bool _InitializeOnAfterCreated();

  bool _UninitializeOnBeforeClose(
    const CefRefPtr<CefBrowser>& browser);

  void _CreateMessageHandlers(
    MessageHandlerSet& handler);

private:
  bool is_closeing_;
  BrowserList browser_list_;
  MessageHandlerSet message_handler_set_;

  CefRefPtr<CefMessageRouterBrowserSide> message_router_;

private:
  ~ApkAnalyzerHandler();

  IMPLEMENT_REFCOUNTING(ApkAnalyzerHandler);

  //
  IMPLEMENT_LOCKING(ApkAnalyzerHandler);
};
}
#endif /* APK_ANALYZER_APK_ANALYZER_HANDLER_H_ */