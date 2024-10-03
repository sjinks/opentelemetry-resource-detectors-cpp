#ifdef _WIN32
#    include <windows.h>
#    include <tchar.h>
#    define MAIN _tmain
#else
#    define MAIN main
#endif

#include <iostream>
#include <memory>
#include <vector>

#include <opentelemetry/sdk/common/attribute_utils.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>
#include <nlohmann/json.hpp>

#ifdef USE_SYSTEM_INCLUDES
#    include <opentelemetry/resource/wwa/container_resource_detector.h>
#    include <opentelemetry/resource/wwa/os_resource_detector.h>
#    include <opentelemetry/resource/wwa/process_resource_detector.h>
#else
#    include "opentelemetry/resource/wwa/container_resource_detector.h"
#    include "opentelemetry/resource/wwa/os_resource_detector.h"
#    include "opentelemetry/resource/wwa/process_resource_detector.h"
#endif

namespace nlohmann {

template<>
struct adl_serializer<opentelemetry::sdk::common::OwnedAttributeValue> {
    static void to_json(json& j, const opentelemetry::sdk::common::OwnedAttributeValue& v)
    {
        opentelemetry::nostd::visit([&j](const auto& value) { j = value; }, v);
    }
};

}  // namespace nlohmann

int MAIN()  // NOLINT(bugprone-exception-escape)
{
    std::vector<std::unique_ptr<opentelemetry::sdk::resource::ResourceDetector>> detectors;
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::container_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::os_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::process_resource_detector>());

    for (const auto& detector : detectors) {
        auto resource = detector->Detect();
        const nlohmann::json j(resource.GetAttributes());
        std::cout << j.dump(4) << "\n";
    }

    return 0;
}
