#pragma once

#if defined(WIN32) || defined(_WIN32)
    #include "windows/User.hpp"
    #include "windows/Executor.hpp"

    namespace Ruby::Platform = Ruby::PlatformWindows;
#elif defined(__linux__) || defined(__unix__)
    // TODO
#endif

namespace Ruby 
{
#if defined(WIN32) || defined(_WIN32)
    using PlatformExecutor = PlatformWindows::WindowsExcecutor;
#elif defined(__linux__) || defined(__unix__)
    // TODO
#endif
}
