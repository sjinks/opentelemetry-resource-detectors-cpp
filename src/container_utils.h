#ifndef AFA594B1_1856_48D6_B6CE_26FB69912583
#define AFA594B1_1856_48D6_B6CE_26FB69912583

#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

std::string get_container_id_from_cgroup_v1(std::istream& stream);
std::string get_container_id_from_cgroup_v2(std::istream& stream);

std::string_view extract_container_id_cgroup_v1(std::string_view line);
std::string_view extract_container_id_cgroup_v2(std::string_view line);

#endif /* AFA594B1_1856_48D6_B6CE_26FB69912583 */
