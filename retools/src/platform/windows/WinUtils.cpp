#include <platform/windows/WinUtils.hpp>


namespace Ruby::PlatformWindows
{
    class WindowsStacktrace
    {

    };


    std::pair<u32, RubyString> getLastError(void)
    {
        u32 code = GetLastError();

        char* msg = nullptr;
        size_t msgSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), msg, 0, NULL);

        return std::make_pair<u32, RubyString>(code, RubyString{ msg, msgSize });
    }


    RubyString getStackTrace(void)
    {

    }
}
