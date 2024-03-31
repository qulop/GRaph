#pragma once

#include "User.hpp"


namespace Ruby::PlatformWindows
{
    std::pair<u32, RubyString> getLastError(void);
    RubyString getStackTrace(void);
}