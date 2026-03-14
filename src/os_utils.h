#ifndef E3E96BBC_E309_4465_AD2D_3F5706202D41
#define E3E96BBC_E309_4465_AD2D_3F5706202D41

#include <string>
#include <string_view>

[[nodiscard]] std::string get_host_arch(std::string_view machine);
[[nodiscard]] std::string get_os_type(std::string_view os);

#endif /* E3E96BBC_E309_4465_AD2D_3F5706202D41 */
