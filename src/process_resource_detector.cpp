#include "opentelemetry/resource/wwa/process_resource_detector.h"

#ifndef _WIN32
#    include <unistd.h>
#else
#    include <array>
#    include <cstdio>

#    include <windows.h>

#    include <fcntl.h>
#    include <io.h>
#    include <lmcons.h>
#    include <processenv.h>
#    include <tlhelp32.h>

#    include "tstring.h"
#endif

#include <opentelemetry/version.h>

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/process_attributes.h>
#    include <opentelemetry/semconv/schema_url.h>
#endif

#include "process_utils.h"

namespace wwa::opentelemetry::resource {

#ifdef _WIN32
int isatty(int fd)
{
    // _get_osfhandle returns -1 on error. Treat invalid handles as non-tty.
    intptr_t osfh = _get_osfhandle(fd);
    if (osfh == -1) {
        return 0;
    }

    HANDLE handle = reinterpret_cast<HANDLE>(osfh);
    if (handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD fileType = GetFileType(handle);
    if (fileType == FILE_TYPE_CHAR) {
        // FILE_TYPE_CHAR typically indicates console or serial; prefer GetConsoleMode
        // to ensure this is an interactive console handle rather than a legacy
        // character device.
        DWORD mode = 0;
        if (GetConsoleMode(handle, &mode) != 0) {
            return 1;
        }

        // If GetConsoleMode fails, conservatively treat as non-tty.
        return 0;
    }

    return 0;
}

DWORD getppid(DWORD pid)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    struct HandleGuard {
        HANDLE handle;
        ~HandleGuard()
        {
            if (this->handle != INVALID_HANDLE_VALUE) {
                CloseHandle(this->handle);
            }
        }
    } guard{hSnapshot};

    DWORD parentPid = 0;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == pid) {
                parentPid = pe32.th32ParentProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    return parentPid;
}
#endif

::opentelemetry::sdk::resource::Resource process_resource_detector::Detect()
{
    ::opentelemetry::sdk::resource::ResourceAttributes attrs;

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessCommand;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessCommandArgs;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessCommandLine;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessGroupLeaderPid;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessInteractive;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessOwner;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessParentPid;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessPid;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserId;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserName;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserId;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserName;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessSessionLeaderPid;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessUserId;
    using ::opentelemetry::sdk::resource::SemanticConventions::kProcessUserName;
    using ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl;
#else
    using ::opentelemetry::semconv::kSchemaUrl;
    using ::opentelemetry::semconv::process::kProcessCommand;
    using ::opentelemetry::semconv::process::kProcessCommandArgs;
    using ::opentelemetry::semconv::process::kProcessCommandLine;
    using ::opentelemetry::semconv::process::kProcessGroupLeaderPid;
    using ::opentelemetry::semconv::process::kProcessInteractive;
    using ::opentelemetry::semconv::process::kProcessOwner;
    using ::opentelemetry::semconv::process::kProcessParentPid;
    using ::opentelemetry::semconv::process::kProcessPid;
    using ::opentelemetry::semconv::process::kProcessRealUserId;
    using ::opentelemetry::semconv::process::kProcessRealUserName;
    using ::opentelemetry::semconv::process::kProcessSavedUserId;
    using ::opentelemetry::semconv::process::kProcessSavedUserName;
    using ::opentelemetry::semconv::process::kProcessSessionLeaderPid;
    using ::opentelemetry::semconv::process::kProcessUserId;
    using ::opentelemetry::semconv::process::kProcessUserName;
#endif

#ifndef _WIN32
    if (const auto cmdline = get_command_line_args(); !cmdline.empty()) {
        attrs[kProcessCommand]     = cmdline[0];
        attrs[kProcessCommandArgs] = cmdline;
    }
#else
    attrs[kProcessCommandLine] = convert(GetCommandLine());
#endif

#ifndef _WIN32
    attrs[kProcessPid]            = getpid();
    attrs[kProcessParentPid]      = getppid();
    attrs[kProcessGroupLeaderPid] = getpgid(0);
    attrs[kProcessInteractive] = isatty(STDIN_FILENO) == 1 || isatty(STDOUT_FILENO) == 1 || isatty(STDERR_FILENO) == 1;
    attrs[kProcessSessionLeaderPid] = getsid(0);
#else
    attrs[kProcessPid]       = static_cast<std::int64_t>(GetCurrentProcessId());
    attrs[kProcessParentPid] = static_cast<std::int64_t>(getppid(GetCurrentProcessId()));

    int stdin_fileno  = _fileno(stdin);
    int stdout_fileno = _fileno(stdout);
    int stderr_fileno = _fileno(stderr);

    attrs[kProcessInteractive] = (stdin_fileno >= 0 && isatty(stdin_fileno) == 1) ||
                                 (stdout_fileno >= 0 && isatty(stdout_fileno) == 1) ||
                                 (stderr_fileno >= 0 && isatty(stderr_fileno) == 1);
#endif

    bool has_uids = true;

#if defined(__APPLE__)
    uid_t ruid = getuid();
    uid_t euid = geteuid();

    attrs[kProcessRealUserId] = ruid;
    attrs[kProcessUserId]     = euid;
#elif defined(__linux__)
    uid_t ruid;  // NOLINT(*-init-variables)
    uid_t euid;  // NOLINT(*-init-variables)
    uid_t suid;  // NOLINT(*-init-variables)

    if (getresuid(&ruid, &euid, &suid) == 0) {
        attrs[kProcessRealUserId]  = ruid;
        attrs[kProcessSavedUserId] = suid;
        attrs[kProcessUserId]      = euid;
    }
    else {
        has_uids = false;
    }
#endif

#ifndef _WIN32
    if (has_uids) {
        if (const auto username = username_by_uid(ruid); !username.empty()) {
            attrs[kProcessRealUserName] = username;
            attrs[kProcessOwner]        = username;
        }

#    ifndef __APPLE__
        if (const auto username = username_by_uid(suid); !username.empty()) {
            attrs[kProcessSavedUserName] = username;
        }
#    endif

        if (const auto username = username_by_uid(euid); !username.empty()) {
            attrs[kProcessUserName] = username;
        }
    }
#else
    std::array<TCHAR, UNLEN + 1> buf;
    if (auto size = static_cast<DWORD>(buf.size()); GetUserName(buf.data(), &size)) {
        std::string username        = convert(buf.data());
        attrs[kProcessRealUserName] = username;
        attrs[kProcessOwner]        = username;
    }
#endif

    return ::opentelemetry::sdk::resource::Resource::Create(attrs, kSchemaUrl);
}

}  // namespace wwa::opentelemetry::resource
