#ifndef APK_ANALYZER_INCLUDE_COMMON_FUNCTION_H_
#define APK_ANALYZER_INCLUDE_COMMON_FUNCTION_H_

#include <atlbase.h>
#include <string>
#include <Shlwapi.h>
#include <windows.h>

#include "..\util.h"

inline void GetProgramDirectoryPath(
  std::string& directory_path) {

  char buffer[kDefaultBufferSize] = { 0 };

  ::GetModuleFileNameA((HMODULE)&__ImageBase, buffer, sizeof(buffer));
  ::PathRemoveFileSpecA(buffer);
  directory_path = buffer;
  directory_path.append("\\");
}
#endif /* APK_ANALYZER_INCLUDE_COMMON_FUNCTION_H_ */