#include "process_utils.h"

#include <fstream>
#include <memory>

#include <pwd.h>
#include <unistd.h>

#ifdef __APPLE__
#    include <array>
#    include <cstring>
#    include <sys/sysctl.h>
#endif

std::vector<std::string> get_command_line_args()
{
#ifdef __APPLE__
    std::array<int, 3> mib{CTL_KERN, KERN_PROCARGS2, getpid()};
    std::size_t len = 0;

    if (sysctl(mib.data(), mib.size(), nullptr, &len, nullptr, 0) == -1) {
        return {};
    }

    std::string cmdline(len, '\0');
    if (sysctl(mib.data(), mib.size(), cmdline.data(), &len, nullptr, 0) == -1) {
        return {};
    }

    cmdline.resize(len);

    int argc = 0;
    if (len < sizeof(argc)) {
        return {};
    }

    std::memcpy(&argc, cmdline.data(), sizeof(argc));

    std::string::size_type start_pos = sizeof(argc);
    std::string::size_type nul_pos   = cmdline.find('\0', start_pos);
    if (nul_pos == std::string::npos) {
        return {};
    }

    start_pos = cmdline.find_first_not_of('\0', nul_pos);
    if (start_pos == std::string::npos) {
        return {};
    }

    std::vector<std::string> result;
    result.reserve(argc);
    while (argc > 0 && nul_pos != std::string::npos) {
        nul_pos = cmdline.find('\0', start_pos);
        result.push_back(cmdline.substr(start_pos, nul_pos - start_pos));
        start_pos = nul_pos + 1;
        --argc;
    }

    return result;
#else
    std::ifstream cmdline_file("/proc/self/cmdline");
    if (!cmdline_file) {
        return {};
    }

    return parse_zero_delimited_stream(cmdline_file);
#endif
}

std::vector<std::string> parse_zero_delimited_stream(std::istream& stream)
{
    std::vector<std::string> result;
    std::string item;
    while (std::getline(stream, item, '\0')) {
        result.push_back(item);
    }

    return result;
}

std::string username_by_uid(uid_t uid)
{
    static long int bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        bufsize = 16384;  // NOLINT(*-magic-numbers)
    }

    // NOLINTNEXTLINE(*-avoid-c-arrays)
    auto buf = std::make_unique<char[]>(bufsize);
    passwd pwd{};
    passwd* result = nullptr;
    getpwuid_r(uid, &pwd, buf.get(), static_cast<std::size_t>(bufsize), &result);
    if (result != nullptr) {
        return result->pw_name;
    }

    return {};
}
