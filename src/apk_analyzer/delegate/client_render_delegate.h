#ifndef APK_ANALYZER_DELEGATE_CLIENT_RENDER_DELEGATE_H_
#define APK_ANALYZER_DELEGATE_CLIENT_RENDER_DELEGATE_H_

#include "base/basictypes.h"
#include "include/wrapper/cef_message_router.h"
#include "../apk_analyzer_app.h"

namespace apk_analyzer {

class ClientRenderDelegate : public virtual ApkAnalyzerApp::RenderDelegate {

public:
  ClientRenderDelegate();

  static void CreateRenderDelegate(
    ApkAnalyzerApp::RenderDelegateSet& delegates);

  virtual void OnWebKitInitialized(
    CefRefPtr<ApkAnalyzerApp> app) OVERRIDE;

  virtual void OnContextCreated(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) OVERRIDE;

  virtual void OnContextReleased(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) OVERRIDE;

  virtual void OnFocusedNodeChanged(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node) OVERRIDE;

  virtual bool OnProcessMessageReceived(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
  bool last_node_is_editable_ = false;
  CefRefPtr<CefMessageRouterRendererSide> message_router_;

private:
  ~ClientRenderDelegate();

  IMPLEMENT_REFCOUNTING(ClientRenderDelegate);
  DISALLOW_COPY_AND_ASSIGN(ClientRenderDelegate);
};
}
#endif /* APK_ANALYZER_DELEGATE_CLIENT_RENDER_DELEGATE_H_ */