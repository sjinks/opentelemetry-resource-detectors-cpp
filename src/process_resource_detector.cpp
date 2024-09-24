#include "opentelemetry_resource_detectors/process_resource_detector.h"

#include <unistd.h>

#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "process_utils.h"

namespace wwa::opentelemetry {

::opentelemetry::sdk::resource::Resource process_resource_detector::Detect()
{
    ::opentelemetry::sdk::resource::ResourceAttributes attrs;

    if (const auto cmdline = get_command_line_args(); !cmdline.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessCommandLine] = cmdline;
    }

    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessPid]            = getpid();
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessParentPid]      = getppid();
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessGroupLeaderPid] = getpgid(0);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessInteractive] =
        isatty(STDIN_FILENO) == 1 || isatty(STDOUT_FILENO) == 1 || isatty(STDERR_FILENO) == 1;
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSessionLeaderPid] = getsid(0);

    uid_t ruid;  // NOLINT(*-init-variables)
    uid_t euid;  // NOLINT(*-init-variables)
    uid_t suid;  // NOLINT(*-init-variables)
    if (getresuid(&ruid, &euid, &suid) == 0) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserId]  = ruid;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserId] = suid;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessUserId]      = euid;
    }

    if (const auto username = username_by_uid(ruid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserName] = username;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessOwner]        = username;
    }

    if (const auto username = username_by_uid(suid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserName] = username;
    }

    if (const auto username = username_by_uid(euid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessUserName] = username;
    }

    return ::opentelemetry::sdk::resource::Resource::Create(
        attrs, ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl
    );
}

}  // namespace wwa::opentelemetry
