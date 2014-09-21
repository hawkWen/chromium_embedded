#ifndef APK_ANALYZER_INCLUDE_COMMON_DEFINE_H_
#define APK_ANALYZER_INCLUDE_COMMON_DEFINE_H_

#include <set>
#include <list>

#include "include/wrapper/cef_message_router.h"

namespace apk_analyzer {

typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

typedef std::list<CefRefPtr<CefBrowser>> BrowserList;

}

#endif /* APK_ANALYZER_INCLUDE_COMMON_DEFINE_H_ */