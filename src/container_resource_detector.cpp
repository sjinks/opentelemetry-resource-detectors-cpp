#include "opentelemetry/resource/wwa/container_resource_detector.h"

#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>

#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "container_utils.h"

namespace wwa::opentelemetry::resource {

::opentelemetry::sdk::resource::Resource container_resource_detector::Detect()
{
    std::string cid;
    if (std::ifstream f("/proc/self/mountinfo"); f) {
        cid = get_container_id_from_cgroup_v2(f);
    }

    if (cid.empty()) {
        if (std::ifstream f("/proc/self/cgroup"); f) {
            cid = get_container_id_from_cgroup_v1(f);
        }
    }

    if (cid.empty()) {
        return ::opentelemetry::sdk::resource::Resource::GetEmpty();
    }

    ::opentelemetry::sdk::resource::ResourceAttributes attrs;
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kContainerId] = cid;

    return ::opentelemetry::sdk::resource::Resource::Create(
        attrs, ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl
    );
}

}  // namespace wwa::opentelemetry::resource
