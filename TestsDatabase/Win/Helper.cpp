#include "Helper.h"

#include <Windows.h>
#include <Shellapi.h>

void openURI(std::string uri)
{
    const char   *curi       = uri.c_str();
    int          size_needed = MultiByteToWideChar(CP_ACP, 0, &curi[0], strlen(curi), NULL, 0);
    std::wstring wstrTo(size_needed, 0);

    MultiByteToWideChar(CP_ACP, 0, &curi[0], strlen(curi), &wstrTo[0], size_needed);

    const char *action = "open";
    size_needed = MultiByteToWideChar(CP_ACP, 0, &action[0], strlen(action), NULL, 0);
    std::wstring wstrTo2(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, &action[0], strlen(action), &wstrTo2[0], size_needed);

    ShellExecute(NULL, wstrTo2.c_str(), wstrTo.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
