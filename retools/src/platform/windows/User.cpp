#include <platform/windows/User.hpp>
#include <utils/SafePtr.hpp>


namespace Ruby::PlatformWindows
{
    RubyString getUsername(void)
    {
        DWORD size = UNLEN + 1;
        SafePtr<char> username{ new char[UNLEN + 1] };

        if (GetUserNameA(username.GetData(), &size))
        {
            // RUBY_ERROR("getUsername() : failed to receive username.");
            return RubyString{ "ERR_USERNAME" };
        }

        return RubyString{ username.GetData(), size };
    }
}
