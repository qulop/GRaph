#pragma once

#include <Definitions.hpp>
#include "Executor.hpp"

#include <Lmcons.h>


namespace Ruby::PlatformWindows
{
    inline RubyString getUsername(void)
    {
        DWORD size = UNLEN + 1;
        char* username = new char[UNLEN + 1];
        
        if (!WindowsExcecutor::Execute(GetUserNameA, username, &size))
        {
            // RUBY_ERROR("getUsername() : failed to recieve username.");
            return RubyString{ "ERR_USRNAME" };
        }
        
        return RubyString{ username, size };
    }
}