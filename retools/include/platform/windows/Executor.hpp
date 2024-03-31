#pragma once

#include <utils/TypeTraits.hpp>
#include <StdInc.hpp>


namespace Ruby::PlatformWindows 
{
    class WindowsExcecutor
    {
    public:
        template<typename Func, typename... Args>
        static decltype(auto) Execute(Func target, Args... args)
        {
            using ReturnType = std::invoke_result_t<Func, Args...>;
            constexpr bool isRetTypeArithmetic = std::is_arithmetic_v<ReturnType>;

            auto&& result = std::invoke(std::forward<Func>(target), std::forward<Args>(args)...);
            if constexpr (std::is_same_v<ReturnType, void> || !std::is_arithmetic_v<ReturnType>)
            {
                if constexpr (std::is_same_v<ReturnType, void>)
                    return true;
                return result;
            }

            if (result)
                return true;
                
            m_lastError.first = GetLastError();
            char* errorMsg = nullptr;
            size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorMsg, 0, NULL);

            m_lastError = RubyString{ errorMsg, m_lastError.first };
            return false;
        }

        static std::pair<u32, RubyString> GetLastError(void);

    private:
        static std::pair<u32, RubyString> m_lastError;
    };
}