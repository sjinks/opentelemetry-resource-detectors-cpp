#ifndef D0FB3391_2C66_44C7_8C7D_82423813E466
#define D0FB3391_2C66_44C7_8C7D_82423813E466

#include <cstddef>
#include <string_view>
#include <vector>

[[nodiscard]] std::vector<std::string_view> split(std::string_view str, char delim);
[[nodiscard]] bool starts_with(std::string_view str, std::string_view prefix) noexcept;
[[nodiscard]] bool ends_with(std::string_view str, std::string_view suffix) noexcept;
[[nodiscard]] std::string_view nth_word(std::string_view str, std::size_t n);

#endif /* D0FB3391_2C66_44C7_8C7D_82423813E466 */
