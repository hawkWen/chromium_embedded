#include "client_render_delegate.h"

#include <new>

namespace apk_analyzer {

  const char kFocusedNodeChangedMessage[] = "ClientRenderer.FocusedNodeChanged";

ClientRenderDelegate::ClientRenderDelegate() {
}

ClientRenderDelegate::~ClientRenderDelegate() {
}

void ClientRenderDelegate::CreateRenderDelegate(
  ApkAnalyzerApp::RenderDelegateSet& delegates) {

  delegates.insert(new (std::nothrow)ClientRenderDelegate);
}

void ClientRenderDelegate::OnWebKitInitialized(
  CefRefPtr<ApkAnalyzerApp> app) {

  // Create the render-side router for query handling
  CefMessageRouterConfig config;

  message_router_ = CefMessageRouterRendererSide::Create(config);
}

void ClientRenderDelegate::OnContextCreated(
  CefRefPtr<ApkAnalyzerApp> app,
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {

  message_router_->OnContextCreated(browser, frame, context);
}

void ClientRenderDelegate::OnContextReleased(
  CefRefPtr<ApkAnalyzerApp> app,
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {
  
  message_router_->OnContextReleased(browser, frame, context);
}

void ClientRenderDelegate::OnFocusedNodeChanged(
  CefRefPtr<ApkAnalyzerApp> app,
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefDOMNode> node) {
  bool is_editable = (node.get() && node->IsEditable());
  if (is_editable != last_node_is_editable_) {
    // Notify the browser of the change in focused element type.
    last_node_is_editable_ = is_editable;
    CefRefPtr<CefProcessMessage> message =
      CefProcessMessage::Create(kFocusedNodeChangedMessage);
    message->GetArgumentList()->SetBool(0, is_editable);
    browser->SendProcessMessage(PID_BROWSER, message);
  }
}

bool ClientRenderDelegate::OnProcessMessageReceived(
  CefRefPtr<ApkAnalyzerApp> app,
  CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {

  return message_router_->OnProcessMessageReceived(browser, source_process, message);
}
}