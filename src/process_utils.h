#ifndef BFE95C0E_7013_4332_8263_104F42F14462
#define BFE95C0E_7013_4332_8263_104F42F14462

#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>
#include <sys/types.h>

std::vector<std::string> get_command_line_args();
std::vector<std::string> parse_zero_delimited_stream(std::istream& stream);
std::string username_by_uid(uid_t uid);

#endif /* BFE95C0E_7013_4332_8263_104F42F14462 */
