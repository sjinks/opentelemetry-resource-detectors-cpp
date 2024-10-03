#ifndef A1E48B9D_065C_4080_9611_64E8339C67B4
#define A1E48B9D_065C_4080_9611_64E8339C67B4

#ifdef _WIN32
#    include <string>
#    include <tchar.h>

inline std::string convert(LPCTSTR s)
{
    if (!s) {
        return {};
    }

#    ifdef _UNICODE
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, s, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed == 0) {
        return {};
    }

    std::string result(static_cast<std::string::size_type>(size_needed), '\0');
    int retval = WideCharToMultiByte(CP_UTF8, 0, s, -1, result.data(), size_needed, nullptr, nullptr);
    if (retval) {
        result.resize(static_cast<std::string::size_type>(retval) - 1);
        return result;
    }

    return {};
#    else
    return {s};
#    endif
}

#endif

#endif /* A1E48B9D_065C_4080_9611_64E8339C67B4 */
