#include "process_utils.h"

#include <fstream>

#ifndef _WIN32
#    include <pwd.h>
#    include <unistd.h>
#endif

#ifdef __APPLE__
#    include <array>
#    include <cstdint>
#    include <cstring>
#    include <limits>
#    include <sys/sysctl.h>
#endif

std::vector<std::string> get_command_line_args()
{
#ifdef __APPLE__
    // KERN_PROCARGS2 returns a buffer that starts with an (int) argc followed
    // by a sequence of '\0' delimited strings. Be defensive: read argc into
    // a fixed-width integer, validate sizes and impose a sane maximum to
    // avoid unbounded allocations if the kernel reports an unexpected size.
    std::array<int, 3> mib{CTL_KERN, KERN_PROCARGS2, getpid()};
    std::size_t len = 0;

    if (sysctl(mib.data(), static_cast<unsigned int>(mib.size()), nullptr, &len, nullptr, 0) == -1) {
        return {};
    }

    if (len == 0) {
        return {};
    }

    // Cap the buffer to a reasonable maximum (1 MiB) to avoid surprises.
    constexpr std::size_t MAX_CMDLINE = 1 << 20;
    if (len > MAX_CMDLINE) {
        // refuse to read extremely large reported sizes
        return {};
    }

    std::string cmdline;
    cmdline.resize(len);
    if (sysctl(mib.data(), static_cast<unsigned int>(mib.size()), cmdline.data(), &len, nullptr, 0) == -1) {
        return {};
    }

    cmdline.resize(len);

    // argc is an int in the kernel layout; read it into a fixed-width type
    // and validate the value before using it to reserve or index.
    if (len < sizeof(std::int32_t)) {
        return {};
    }

    std::int32_t raw_argc = 0;
    std::memcpy(&raw_argc, cmdline.data(), sizeof(raw_argc));
    if (raw_argc <= 0) {
        return {};
    }

    // Sanity cap for argc to avoid pathological values
    constexpr std::int32_t MAX_ARGC = 4096;
    if (raw_argc > MAX_ARGC) {
        return {};
    }

    const int argc = static_cast<int>(raw_argc);

    std::string::size_type start_pos = sizeof(raw_argc);
    if (start_pos >= cmdline.size()) {
        return {};
    }

    std::string::size_type nul_pos = cmdline.find('\0', start_pos);
    if (nul_pos == std::string::npos) {
        return {};
    }

    start_pos = cmdline.find_first_not_of('\0', nul_pos);
    if (start_pos == std::string::npos) {
        return {};
    }

    std::vector<std::string> result;
    result.reserve(static_cast<std::size_t>(argc));
    int remaining = argc;
    while (remaining > 0 && start_pos < cmdline.size()) {
        nul_pos = cmdline.find('\0', start_pos);
        if (nul_pos == std::string::npos) {
            // final token: take rest of buffer
            result.push_back(cmdline.substr(start_pos));
            break;
        }

        result.push_back(cmdline.substr(start_pos, nul_pos - start_pos));
        start_pos = nul_pos + 1;
        --remaining;
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

#ifndef _WIN32
std::string username_by_uid(uid_t uid)
{
    constexpr auto BUF_FALLBACK_SIZE = 16384;
    static long int bufsize          = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        bufsize = BUF_FALLBACK_SIZE;
    }

    std::string buf(static_cast<std::size_t>(bufsize), '\0');
    passwd pwd{};
    passwd* result = nullptr;
    int rc         = getpwuid_r(uid, &pwd, buf.data(), static_cast<std::size_t>(bufsize), &result);

    if (result != nullptr && rc == 0) {  // NOSONAR
        return result->pw_name;
    }

    return {};
}
#endif
