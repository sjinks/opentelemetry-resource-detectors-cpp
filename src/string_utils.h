#ifndef D0FB3391_2C66_44C7_8C7D_82423813E466
#define D0FB3391_2C66_44C7_8C7D_82423813E466

#include <cstddef>
#include <string_view>
#include <vector>

std::vector<std::string_view> split(std::string_view str, char delim);
bool starts_with(std::string_view str, std::string_view prefix);
bool ends_with(std::string_view str, std::string_view suffix);
std::string_view remove_prefix(std::string_view s, std::string_view prefix);
std::string_view nth_word(std::string_view str, std::size_t n);

#endif /* D0FB3391_2C66_44C7_8C7D_82423813E466 */
