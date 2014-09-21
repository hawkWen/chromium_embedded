#ifndef APK_ANALYZER_APK_ANALYZER_APP_H_
#define APK_ANALYZER_APK_ANALYZER_APP_H_

#include <set>

#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

namespace apk_analyzer {

class ApkAnalyzerApp : public CefApp,
                       public CefBrowserProcessHandler,
                       public CefRenderProcessHandler {

//////////////////////////////////////////////////////////////////////////
//
public:
class RenderDelegate : public virtual CefBase {

public:
  virtual void OnRenderThreadCreated(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefListValue> extra_info) {};

  virtual void OnWebKitInitialized(
    CefRefPtr<ApkAnalyzerApp> app) {};

  virtual void OnBrowserCreated(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser) {};

  virtual void OnBrowserDestroyed(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser) {};

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler(
    CefRefPtr<ApkAnalyzerApp> app) { return nullptr; };

  virtual bool OnBeforeNavigation(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    NavigationType navigation_type,
    bool is_redirect) { return false; };

  virtual void OnContextCreated(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) {};

  virtual void OnContextReleased(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) {};

  virtual void OnUncaughtException(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context,
    CefRefPtr<CefV8Exception> exception,
    CefRefPtr<CefV8StackTrace> stack_trace) {};

  virtual void OnFocusedNodeChanged(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node) {};

  virtual bool OnProcessMessageReceived(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) { return false; };
};
typedef std::set<CefRefPtr<RenderDelegate>> RenderDelegateSet;

class BrowserDelegate : public virtual CefBase {

public:
  virtual void OnContextInitialized(
    CefRefPtr<ApkAnalyzerApp> app) {};

  virtual void OnBeforeChildProcessLaunch(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefCommandLine> command_line) {};

  virtual void OnRenderProcessThreadCreated(
    CefRefPtr<ApkAnalyzerApp> app,
    CefRefPtr<CefListValue> extra_info) {};
};
typedef std::set<CefRefPtr<BrowserDelegate>> BrowserDelegateSet;
//////////////////////////////////////////////////////////////////////////
public:
  ApkAnalyzerApp();

  //////////////////////////////////////////////////////////////////////////
  // CefApp
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE;

  //////////////////////////////////////////////////////////////////////////
  // CefBrowserProcessHandler

  // Called on the browser process UI thread immediately after the CEF context
  // has been initialize
  virtual void OnContextInitialized() OVERRIDE;

  // Called before a child process is launched. Will be called on the browser
  // process UI thread when launching a render process and on the browser
  // process IO thread when launching a GPU or plugin process. Provides an
  // opportunity to modify the child process command line. Do not keep a
  // reference to |command_line| outside of this methods
  virtual void OnBeforeChildProcessLaunch(
    CefRefPtr<CefCommandLine> command_line) OVERRIDE;

  // Called on the browser process IO thread after the main thread has been
  // created for a new render process. Provides an opportunity to specify extra
  // information that will be passed to
  // CefRenderProcessHandler::OnRenderThreadCreated() in the render process. Do
  // not keep a reference to |extra_info| outside of this method
  virtual void OnRenderProcessThreadCreated(
    CefRefPtr<CefListValue> extra_info) OVERRIDE;


  //////////////////////////////////////////////////////////////////////////
  // CefRenderProcessHandler

  // Called after the render process main thread has been created. |extra_info|
  // is a read-only value originating from
  // CefBrowserProcessHandler::OnRenderProcessThreadCreated(). Do not keep a
  // reference to |extra_info| outside of this method
  virtual void OnRenderThreadCreated(
    CefRefPtr<CefListValue> extra_info) OVERRIDE;

  // Called after WebKit has been initialized
  virtual void OnWebKitInitialized() OVERRIDE;

  // Called after a browser has been created. When browsing cross-origin a new
  // browser will be created before the old browser with the same identifier is 
  // destroyed
  virtual void OnBrowserCreated(
    CefRefPtr<CefBrowser> browser) OVERRIDE;

  // Called before a browser is destroyed
  virtual void OnBrowserDestroyed(
    CefRefPtr<CefBrowser> browser) OVERRIDE;

  // Return the handler for browser load status events.
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

  // Called before browser navigation. Return true to cancel the navigation or 
  // false to allow the navigation to proceed. The |request| object cannot be
  // modified in this callback
  virtual bool OnBeforeNavigation(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    NavigationType navigation_type,
    bool is_redirect) OVERRIDE;

  // Called immediately after the V8 context for a frame has been created. To
  // retrieve the JavaScript 'window' object use the CefV8Context::GetGlobal()
  // method. V8 Handles can only be accessed from the thread on which they are
  // created. A task runner for posting tasks on the associated thread can be
  // retrieved via the CefV8Context::GetTaskRunner() method.
  virtual void OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) OVERRIDE;

  // Called immediately before the V8 context for a frame is released. No
  // references to the context should be kept after this method is called
  virtual void OnContextReleased(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context) OVERRIDE;

  // Called for global uncaught exceptions in a frame. Execution of this
  // callback is disabled by default. To enable set
  // CefSettings.uncaught_exception_stack_size > 0.
  virtual void OnUncaughtException(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context,
    CefRefPtr<CefV8Exception> exception,
    CefRefPtr<CefV8StackTrace> stack_trace) OVERRIDE;

  // Called when a new node in the browser gets focus. The |node| value may
  // be empty if no specific node has gained focus. The node object passed to
  // this method represents a snapshot of the DOM at the time this method is
  // executed. DOM objects are only valid for the scope of this method. Do not 
  // keep references to or attempt to access any DOM objects outside the scope
  // of this method
  virtual void OnFocusedNodeChanged(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node) OVERRIDE;

  // Called when a new message is received from a different process. Return true
  // if the message was handled or false otherwise. Do not keep a reference to
  // or attempt to access the message outside of this callback
  virtual bool OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
  inline void _CreateRenderDelegates(
    RenderDelegateSet& delegates);

private:
  // Set of supported RenderDelegates. Only used in the renderer process
  RenderDelegateSet render_delegates_;
private:
  ~ApkAnalyzerApp();

  //Include the default reference counting implementation
  IMPLEMENT_REFCOUNTING(ApkAnalyzerApp);
  DISALLOW_COPY_AND_ASSIGN(ApkAnalyzerApp);
};
}
#endif /* APK_ANALYZER_APK_ANALYZER_APP_H_ */