#include "opentelemetry/resource/wwa/container_resource_detector.h"

#include <fstream>
#include <string>

#include <opentelemetry/version.h>

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/container_attributes.h>
#    include <opentelemetry/semconv/schema_url.h>
#endif

#include "container_utils.h"

namespace wwa::opentelemetry::resource {

::opentelemetry::sdk::resource::Resource container_resource_detector::Detect()
{
#ifndef __linux__
    return ::opentelemetry::sdk::resource::Resource::GetEmpty();
#else
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

#    if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using ::opentelemetry::sdk::resource::SemanticConventions::kContainerId;
    using ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl;
#    else
    using ::opentelemetry::semconv::container::kContainerId;
    using ::opentelemetry::semconv::kSchemaUrl;
#    endif

    ::opentelemetry::sdk::resource::ResourceAttributes attrs;
    attrs[kContainerId] = cid;

    return ::opentelemetry::sdk::resource::Resource::Create(attrs, kSchemaUrl);
#endif
}

}  // namespace wwa::opentelemetry::resource
