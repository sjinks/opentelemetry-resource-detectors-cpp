#include "opentelemetry/resource/wwa/process_resource_detector.h"

#ifndef _WIN32
#    include <unistd.h>
#else
#    include <array>
#    include <cstdio>

#    include <windows.h>

#    include <Lmcons.h>
#    include <fcntl.h>
#    include <io.h>
#    include <processenv.h>
#    include <tlhelp32.h>

#    include "tstring.h"
#endif

#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "process_utils.h"

namespace wwa::opentelemetry::resource {

#ifdef _WIN32
int isatty(int fd)
{
    DWORD fileType = GetFileType(reinterpret_cast<HANDLE>(_get_osfhandle(fd)));
    return fileType == FILE_TYPE_CHAR ? 1 : 0;
}

DWORD getppid(DWORD pid)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

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

    CloseHandle(hSnapshot);
    return parentPid;
}
#endif

::opentelemetry::sdk::resource::Resource process_resource_detector::Detect()
{
    ::opentelemetry::sdk::resource::ResourceAttributes attrs;

#ifndef _WIN32
    if (const auto cmdline = get_command_line_args(); !cmdline.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessCommand]     = cmdline[0];
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessCommandArgs] = cmdline;
    }
#else
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessCommandLine] = convert(GetCommandLine());
#endif

#ifndef _WIN32
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessPid]            = getpid();
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessParentPid]      = getppid();
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessGroupLeaderPid] = getpgid(0);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessInteractive] =
        isatty(STDIN_FILENO) == 1 || isatty(STDOUT_FILENO) == 1 || isatty(STDERR_FILENO) == 1;
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSessionLeaderPid] = getsid(0);
#else
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessPid] =
        static_cast<std::int64_t>(GetCurrentProcessId());
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessParentPid] =
        static_cast<std::int64_t>(getppid(GetCurrentProcessId()));

    int stdin_fileno  = _fileno(stdin);
    int stdout_fileno = _fileno(stdout);
    int stderr_fileno = _fileno(stderr);

    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessInteractive] =
        (stdin_fileno >= 0 && isatty(stdin_fileno) == 1) || (stdout_fileno >= 0 && isatty(stdout_fileno) == 1) ||
        (stderr_fileno >= 0 && isatty(stderr_fileno) == 1);
#endif

#if defined(__APPLE__)
    uid_t ruid = getuid();
    uid_t euid = geteuid();

    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserId] = ruid;
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessUserId]     = euid;
#elif defined(__linux__)
    uid_t ruid;  // NOLINT(*-init-variables)
    uid_t euid;  // NOLINT(*-init-variables)
    uid_t suid;  // NOLINT(*-init-variables)

    if (getresuid(&ruid, &euid, &suid) == 0) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserId]  = ruid;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserId] = suid;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessUserId]      = euid;
    }
#endif

#ifndef _WIN32
    if (const auto username = username_by_uid(ruid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserName] = username;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessOwner]        = username;
    }

#    ifndef __APPLE__
    if (const auto username = username_by_uid(suid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessSavedUserName] = username;
    }
#    endif

    if (const auto username = username_by_uid(euid); !username.empty()) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessUserName] = username;
    }
#else
    std::array<TCHAR, UNLEN + 1> buf;
    if (auto size = static_cast<DWORD>(buf.size()); GetUserName(buf.data(), &size)) {
        std::string username                                                             = convert(buf.data());
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessRealUserName] = username;
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kProcessOwner]        = username;
    }
#endif

    return ::opentelemetry::sdk::resource::Resource::Create(
        attrs, ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl
    );
}

}  // namespace wwa::opentelemetry::resource
