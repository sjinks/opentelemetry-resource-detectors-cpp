#include "process_utils.h"

#include <fstream>
#include <memory>

#include <pwd.h>
#include <unistd.h>

std::vector<std::string> get_command_line_args()
{
    std::ifstream cmdline_file("/proc/self/cmdline");
    if (!cmdline_file) {
        return {};
    }

    return parse_zero_delimited_stream(cmdline_file);
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
