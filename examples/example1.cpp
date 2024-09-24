#include <iostream>
#include <memory>
#include <vector>

#include <opentelemetry/sdk/common/attribute_utils.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>
#include <nlohmann/json.hpp>

#include "opentelemetry_resource_detectors/container_resource_detector.h"
#include "opentelemetry_resource_detectors/os_resource_detector.h"
#include "opentelemetry_resource_detectors/process_resource_detector.h"

namespace nlohmann {

template<>
struct adl_serializer<opentelemetry::sdk::common::OwnedAttributeValue> {
    static void to_json(json& j, const opentelemetry::sdk::common::OwnedAttributeValue& v)
    {
        opentelemetry::nostd::visit([&j](const auto& value) { j = value; }, v);
    }
};

}  // namespace nlohmann

int main()  // NOLINT(bugprone-exception-escape)
{
    std::vector<std::unique_ptr<opentelemetry::sdk::resource::ResourceDetector>> detectors;
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::container_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::os_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::process_resource_detector>());

    for (const auto& detector : detectors) {
        auto resource = detector->Detect();
        const nlohmann::json j(resource.GetAttributes());
        std::cout << j.dump(4) << "\n";
    }

    return 0;
}
