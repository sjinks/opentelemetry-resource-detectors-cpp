#ifndef BFE95C0E_7013_4332_8263_104F42F14462
#define BFE95C0E_7013_4332_8263_104F42F14462

#include <iosfwd>
#include <string>
#include <vector>
#include <sys/types.h>

[[nodiscard]] std::vector<std::string> get_command_line_args();
[[nodiscard]] std::vector<std::string> parse_zero_delimited_stream(std::istream& stream);
#ifndef _WIN32
[[nodiscard]] std::string username_by_uid(uid_t uid);
#endif

#endif /* BFE95C0E_7013_4332_8263_104F42F14462 */
