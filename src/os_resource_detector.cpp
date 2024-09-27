#include "opentelemetry/resource/wwa/os_resource_detector.h"

#include <string>
#include <string_view>

#include <sys/utsname.h>

#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "os_utils.h"

namespace wwa::opentelemetry::resource {

::opentelemetry::sdk::resource::Resource os_resource_detector::Detect()
{
    utsname info{};
    if (uname(&info) == -1) {
        return ::opentelemetry::sdk::resource::Resource::GetEmpty();
    }

    ::opentelemetry::sdk::resource::ResourceAttributes attrs;
    // NOLINTBEGIN(*-bounds-array-to-pointer-decay) -- we have no control over the system interface
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostArch]  = get_host_arch(info.machine);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostName]  = std::string(info.nodename);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsType]    = get_os_type(info.sysname);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsVersion] = std::string(info.release);
    // NOLINTEND(*-bounds-array-to-pointer-decay)

    return ::opentelemetry::sdk::resource::Resource::Create(
        attrs, ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl
    );
}

}  // namespace wwa::opentelemetry::resource
