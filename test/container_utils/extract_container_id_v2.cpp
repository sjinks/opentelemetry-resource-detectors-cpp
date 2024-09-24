#include <tuple>

#include <gtest/gtest.h>

#include "container_utils.h"

class ParametrizedExtractContainerIDV2Test
    : public testing::TestWithParam<std::tuple<std::string_view, std::string_view>> {};

TEST_P(ParametrizedExtractContainerIDV2Test, ExtractContainerIDV2)
{
    std::string_view input;
    std::string_view expected;
    std::tie(input, expected) = GetParam();

    auto actual = extract_container_id_cgroup_v2(input);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedExtractContainerIDV2Test,
    testing::Values(
        std::make_tuple(
            "4245 4237 0:94 "
            "/containers/overlay-containers/4e9dc37d00ebd2daea029d84bb37764ce12d746a6f3a33c5969cee15c4fc4418/userdata/"
            "hostname /etc/hostname rw,nosuid,nodev,relatime - tmpfs tmpfs "
            "rw,size=6557728k,nr_inodes=1639432,mode=700,uid=1000,gid=1000,inode64",
            "4e9dc37d00ebd2daea029d84bb37764ce12d746a6f3a33c5969cee15c4fc4418"
        ),
        std::make_tuple(
            "2866 2837 252:0 "
            "/var/lib/docker/containers/42ea5f70df43ed01693d73c23381ef1ec7451115e6aa5f60a51adef991e27d38/hostname "
            "/etc/hostname rw,relatime - ext4 /dev/mapper/vg01-root rw,discard,errors=remount-ro",
            "42ea5f70df43ed01693d73c23381ef1ec7451115e6aa5f60a51adef991e27d38"
        ),

        std::make_tuple(
            "4247 4237 0:94 "
            "/containers/overlay-containers/4e9dc37d00ebd2daea029d84bb37764ce12d746a6f3a33c5969cee15c4fc4418/userdata/"
            "hosts /etc/hosts rw,nosuid,nodev,relatime - tmpfs tmpfs rw",
            ""
        ),
        std::make_tuple("4051 4238 0:134 /bus /proc/bus ro,nosuid,nodev,noexec,relatime - proc proc rw", ""),
        std::make_tuple("", "")
    )
);
