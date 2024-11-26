#include "opentelemetry/resource/wwa/os_resource_detector.h"

#include <string>
#include <string_view>

#ifndef _WIN32
#    include <sys/utsname.h>
#else
#    include <array>

#    include <windows.h>

#    include "tstring.h"
#endif

#include <opentelemetry/version.h>

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/host_attributes.h>
#    include <opentelemetry/semconv/incubating/os_attributes.h>
#    include <opentelemetry/semconv/schema_url.h>
#endif

#include "os_utils.h"

namespace wwa::opentelemetry::resource {

::opentelemetry::sdk::resource::Resource os_resource_detector::Detect()
{
    ::opentelemetry::sdk::resource::ResourceAttributes attrs;

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using ::opentelemetry::sdk::resource::SemanticConventions::kHostArch;
    using ::opentelemetry::sdk::resource::SemanticConventions::kHostName;
    using ::opentelemetry::sdk::resource::SemanticConventions::kOsType;
    using ::opentelemetry::sdk::resource::SemanticConventions::kOsVersion;
    using ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl;

    using namespace ::opentelemetry::sdk::resource::SemanticConventions::HostArchValues;
    using namespace ::opentelemetry::sdk::resource::SemanticConventions::OsTypeValues;
#else
    using ::opentelemetry::semconv::host::kHostArch;
    using ::opentelemetry::semconv::host::kHostName;
    using ::opentelemetry::semconv::kSchemaUrl;
    using ::opentelemetry::semconv::os::kOsType;
    using ::opentelemetry::semconv::os::kOsVersion;

    using namespace ::opentelemetry::semconv::host::HostArchValues;
    using namespace ::opentelemetry::semconv::os::OsTypeValues;
#endif

#ifndef _WIN32
    utsname info{};
    if (uname(&info) == -1) {
        return ::opentelemetry::sdk::resource::Resource::GetEmpty();
    }

    // NOLINTBEGIN(*-bounds-array-to-pointer-decay) -- we have no control over the system interface
    attrs[kHostArch]  = get_host_arch(info.machine);
    attrs[kHostName]  = std::string(info.nodename);
    attrs[kOsType]    = get_os_type(info.sysname);
    attrs[kOsVersion] = std::string(info.release);
    // NOLINTEND(*-bounds-array-to-pointer-decay)
#else
    SYSTEM_INFO si{};
    GetSystemInfo(&si);
    switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            attrs[kHostArch] = std::string(kAmd64);
            break;

        case PROCESSOR_ARCHITECTURE_INTEL:
            attrs[kHostArch] = std::string(kX86);
            break;

        case PROCESSOR_ARCHITECTURE_ARM64:
            attrs[kHostArch] = std::string(kArm64);
            break;

        default:
            break;
    }

    std::array<TCHAR, MAX_COMPUTERNAME_LENGTH + 1> buf;
    if (auto size = static_cast<DWORD>(buf.size()); GetComputerName(buf.data(), &size)) {
        attrs[kHostName] = convert(buf.data());
    }

    attrs[kOsType] = std::string(kWindows);

    OSVERSIONINFO osvi{};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    if (GetVersionEx(&osvi)) {
        attrs[kOsVersion] = std::to_string(osvi.dwMajorVersion)
                                .append(".")
                                .append(std::to_string(osvi.dwMinorVersion))
                                .append(" (Build ")
                                .append(std::to_string(osvi.dwBuildNumber))
                                .append(")");
    }
#endif

    return ::opentelemetry::sdk::resource::Resource::Create(attrs, kSchemaUrl);
}

}  // namespace wwa::opentelemetry::resource
