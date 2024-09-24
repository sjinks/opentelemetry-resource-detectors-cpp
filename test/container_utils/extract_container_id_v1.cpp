#include <tuple>

#include <gtest/gtest.h>

#include "container_utils.h"

class ParametrizedExtractContainerIDV1Test
    : public testing::TestWithParam<std::tuple<std::string_view, std::string_view>> {};

TEST_P(ParametrizedExtractContainerIDV1Test, ExtractContainerIDV1)
{
    std::string_view input;
    std::string_view expected;
    std::tie(input, expected) = GetParam();

    auto actual = extract_container_id_cgroup_v1(input);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedExtractContainerIDV1Test,
    testing::Values(
        std::make_tuple(
            "0::/system.slice/docker-2d0bdb07d9875685a9444722fb0c9a5a602c7c0fb43df67bc15298d65f55d7ca.scope/"
            "kubepods.slice/kubepods-besteffort.slice/"
            "kubepods-besteffort-podccb388c7_7a47_44fe_8a22_c4ba2e3cb768.slice/"
            "docker-878d41c03caa1b1033c1e7cb6c5ed75aafa2673d8aaaae5025e959a7b5c5dc38.scope",
            "878d41c03caa1b1033c1e7cb6c5ed75aafa2673d8aaaae5025e959a7b5c5dc38"
        ),
        std::make_tuple(
            "10:pids:/kubepods.slice/kubepods-burstable.slice/"
            "kubepods-burstable-podade50c4e_141d_4b61_a154_c835f04f0d73.slice/"
            "cri-containerd-bdc11dc1d24720bfac0c05040d3d54f0525c64e0275f4e7d9028711504b4fac7.scope",
            "bdc11dc1d24720bfac0c05040d3d54f0525c64e0275f4e7d9028711504b4fac7"
        ),
        std::make_tuple(
            "0::/kubepods.slice/kubepods-besteffort.slice/"
            "kubepods-besteffort-pod3d1b4f66_e156_4950_a838_c4d71c423e81.slice/"
            "docker-2bb91674d621cab821417b69dc96b12de89daeed340852e7dd48c82ed45efcf5.scope",
            "2bb91674d621cab821417b69dc96b12de89daeed340852e7dd48c82ed45efcf5"
        ),
        std::make_tuple(
            "12:hugetlb:/kubepods/burstable/pod1a32b976-4e23-459d-8925-b71621b1c339/"
            "2cfe3b181e6065cf064f546ae953d0a639113cea821ca770abf266db5c508fa8",
            "2cfe3b181e6065cf064f546ae953d0a639113cea821ca770abf266db5c508fa8"
        ),
        std::make_tuple(
            "0::/system.slice/docker-9c9232d7f4cffac0ccf08891af7f3c0ecd9fddec31fa757af4667df468dc4037.scope",
            "9c9232d7f4cffac0ccf08891af7f3c0ecd9fddec31fa757af4667df468dc4037"
        ),
        std::make_tuple(
            "0::/machine.slice/libpod-c8ea7b20809fd35b75b56851d7733f957abb3e368b5a4d221b1352895ccab17a.scope",
            "c8ea7b20809fd35b75b56851d7733f957abb3e368b5a4d221b1352895ccab17a"
        ),

        std::make_tuple(
            "12:hugetlb:/kubepods.slice/kubepods-besteffort.slice/kubepods-besteffort-pod1234abc.scope/crio-5678ef91",
            "5678ef91"
        ),
        std::make_tuple(
            "12:hugetlb:/docker/8d97ac4ff63c6c7098b12dbd1b89e06bb93b05e99389cfad7765e1e76b72e1a2",
            "8d97ac4ff63c6c7098b12dbd1b89e06bb93b05e99389cfad7765e1e76b72e1a2"
        ),
        std::make_tuple(
            "8:cpuset:/kubepods.slice/kubepods-burstable.slice/kubepods-burstable-pod5678efgh.scope/"
            "containerd-1234abcd",
            "1234abcd"
        ),
        std::make_tuple(
            "11:devices:/kubepods.slice/kubepods-besteffort.slice/"
            "kubepods-besteffort-pod5c5979ec_6b2b_11e9_a923_42010a800002.slice/crio-1234567890abcdef.scope'",
            "1234567890abcdef"
        ),
        std::make_tuple(
            "11:devices:/kubepods/burstable/pod2c4b2241-5c01-11e9-8e4e-42010a800002/"
            "cri-containerd-1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef",
            "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"
        ),
        std::make_tuple(
            "0::/system.slice/containerd.service/"
            "kubepods-burstable-pod2c4b2241-5c01-11e9-8e4e-42010a800002.slice:cri-containerd:1234567890abcdef",
            "1234567890abcdef"
        ),
        std::make_tuple(
            "0::/system.slice/containerd.service/"
            "kubepods-burstable-pod2c4b2241-5c01-11e9-8e4e-42010a800002.slice:cri-containerd-1234567890abcdef.extra",
            "1234567890abcdef"
        ),
        std::make_tuple("0::/", ""),
        std::make_tuple("", ""),
        std::make_tuple("11::/xxx/invalid.scope", ""),
        std::make_tuple(
            "1::/docker/1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef.suffix",
            "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"
        )
    )
);
