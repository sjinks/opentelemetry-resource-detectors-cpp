#ifndef A1E48B9D_065C_4080_9611_64E8339C67B4
#define A1E48B9D_065C_4080_9611_64E8339C67B4

#ifdef _WIN32
#    include <iostream>
#    include <string>
#    include <vector>
#    include <tchar.h>

inline std::string convert(LPCWSTR s)
{
    if (!s) {
        return {};
    }

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
}

inline std::string convert(LPCSTR s)
{
    if (!s) {
        return {};
    }

    int size_needed = MultiByteToWideChar(CP_ACP, 0, s, -1, nullptr, 0);
    if (size_needed == 0) {
        return {};
    }

    std::vector<wchar_t> buffer(size_needed);
    int retval = MultiByteToWideChar(CP_ACP, 0, s, -1, buffer.data(), size_needed);
    if (retval == 0) {
        return {};
    }

    return convert(buffer.data());
}

#endif

#endif /* A1E48B9D_065C_4080_9611_64E8339C67B4 */
