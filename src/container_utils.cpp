#include "container_utils.h"

#include <algorithm>
#include <cstring>
#include <istream>

#include "string_utils.h"

namespace {

using extractor_t = std::string_view (*)(std::string_view);

std::string get_container_id(std::istream& stream, extractor_t extractor)
{
    std::string line;
    while (std::getline(stream, line)) {
        if (const auto cid = extractor(line); !cid.empty()) {
            return std::string(cid);
        }
    }

    return {};
}

}  // namespace

std::string_view extract_container_id_cgroup_v1(std::string_view line)
{
    // minikube+docker kubernetes cluster
    // 0::/system.slice/docker-2d0bdb07d9875685a9444722fb0c9a5a602c7c0fb43df67bc15298d65f55d7ca.scope/kubepods.slice/kubepods-besteffort.slice/kubepods-besteffort-podccb388c7_7a47_44fe_8a22_c4ba2e3cb768.slice/docker-878d41c03caa1b1033c1e7cb6c5ed75aafa2673d8aaaae5025e959a7b5c5dc38.scope
    //
    // regular kubernetes clusters
    // 10:pids:/kubepods.slice/kubepods-burstable.slice/kubepods-burstable-podade50c4e_141d_4b61_a154_c835f04f0d73.slice/cri-containerd-bdc11dc1d24720bfac0c05040d3d54f0525c64e0275f4e7d9028711504b4fac7.scope
    // 0::/kubepods.slice/kubepods-besteffort.slice/kubepods-besteffort-pod3d1b4f66_e156_4950_a838_c4d71c423e81.slice/docker-2bb91674d621cab821417b69dc96b12de89daeed340852e7dd48c82ed45efcf5.scope
    // 12:hugetlb:/kubepods/burstable/pod1a32b976-4e23-459d-8925-b71621b1c339/2cfe3b181e6065cf064f546ae953d0a639113cea821ca770abf266db5c508fa8
    //
    // $ docker run -it --cgroupns host alpine cat /proc/self/cgroup
    // 0::/system.slice/docker-9c9232d7f4cffac0ccf08891af7f3c0ecd9fddec31fa757af4667df468dc4037.scope
    //
    // $ podman run -it --cgroupns host alpine cat /proc/self/cgroup
    // 0::/machine.slice/libpod-c8ea7b20809fd35b75b56851d7733f957abb3e368b5a4d221b1352895ccab17a.scope
    const auto parts = split(line, '/');
    if (parts.size() == 1) {
        return {};
    }

    auto last_part = parts.back();
    if (const auto idx = last_part.rfind(':'); idx != std::string_view::npos) {
        last_part = last_part.substr(idx + 1);
    }

    if (starts_with(last_part, "crio-")) {
        last_part.remove_prefix(std::strlen("crio-"));
    }
    else if (starts_with(last_part, "docker-")) {
        last_part.remove_prefix(std::strlen("docker-"));
    }
    else if (starts_with(last_part, "libpod-")) {
        last_part.remove_prefix(std::strlen("libpod-"));
    }
    else if (starts_with(last_part, "containerd-")) {
        last_part.remove_prefix(std::strlen("containerd-"));
    }
    else if (starts_with(last_part, "cri-containerd-")) {
        last_part.remove_prefix(std::strlen("cri-containerd-"));
    }

    if (const auto idx = last_part.rfind('.'); idx != std::string_view::npos) {
        last_part = last_part.substr(0, idx);
    }

    // NOLINTNEXTLINE(*-magic-numbers)
    return last_part.size() >= 8 && last_part.find_first_not_of("0123456789abcdefABCDEF") == std::string_view::npos
               ? last_part
               : std::string_view{};
}

/**
 * @see https://man7.org/linux/man-pages/man5/proc_pid_mountinfo.5.html
 */
std::string_view extract_container_id_cgroup_v2(std::string_view line)
{
    // podman:
    // 4245 4237 0:94
    // /containers/overlay-containers/4e9dc37d00ebd2daea029d84bb37764ce12d746a6f3a33c5969cee15c4fc4418/userdata/hostname
    // /etc/hostname rw,nosuid,nodev,relatime - tmpfs tmpfs
    // rw,size=6557728k,nr_inodes=1639432,mode=700,uid=1000,gid=1000,inode64
    //
    // docker:
    // 2866 2837 252:0
    // /var/lib/docker/containers/42ea5f70df43ed01693d73c23381ef1ec7451115e6aa5f60a51adef991e27d38/hostname
    // /etc/hostname rw,relatime - ext4 /dev/mapper/vg01-root rw,discard,errors=remount-ro
    //
    // containerd:
    // 1896 1887 9:2
    // /var/lib/rancher/k3s/agent/containerd/io.containerd.grpc.v1.cri/sandboxes/90240a2dd000f4e016b84aa8989a3e578dee59730dd9e62a34760235f2dea0c8/hostname
    // /etc/hostname rw,relatime - ext4 /dev/md2 rw but 90240a2dd000f4e016b84aa8989a3e578dee59730dd9e62a34760235f2dea0c8
    // is not a container id

    // MountID ParentID Major:Minor Root
    if (auto root = nth_word(line, 4); ends_with(root, "/hostname")) {
        const auto parts = split(root, '/');
        auto it          = std::adjacent_find(parts.begin(), parts.end(), [](const auto& a, const auto& b) {
            // NOLINTNEXTLINE(*-magic-numbers)
            return (a == "containers" || a == "overlay-containers") && b.size() == 64 &&
                   b.find_first_not_of("0123456789abcdefABCDEF") == std::string_view::npos;
        });

        if (it != parts.end()) {
            return *(it + 1);
        }
    }

    return {};
}

std::string get_container_id_from_cgroup_v1(std::istream& stream)
{
    return get_container_id(stream, extract_container_id_cgroup_v1);
}

std::string get_container_id_from_cgroup_v2(std::istream& stream)
{
    return get_container_id(stream, extract_container_id_cgroup_v2);
}
