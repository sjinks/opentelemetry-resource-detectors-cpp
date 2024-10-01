#include <iostream>
#include <memory>
#include <vector>

#include <opentelemetry/exporters/ostream/span_exporter_factory.h>
#include <opentelemetry/sdk/common/attribute_utils.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>
#include <opentelemetry/sdk/trace/simple_processor.h>
#include <opentelemetry/sdk/trace/simple_processor_factory.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/provider.h>
#include <nlohmann/json.hpp>

#include "opentelemetry/resource/wwa/container_resource_detector.h"
#include "opentelemetry/resource/wwa/os_resource_detector.h"
#include "opentelemetry/resource/wwa/process_resource_detector.h"

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
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::container_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::os_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::resource::process_resource_detector>());

    auto resource = opentelemetry::sdk::resource::Resource::Create({});

    for (const auto& detector : detectors) {
        auto detected = detector->Detect();
        resource      = resource.Merge(detected);
    }

    const nlohmann::json j(resource.GetAttributes());
    std::cout << j.dump(4) << "\n";

    // Now that the resource is ready, pass it to providers

    auto exporter  = opentelemetry::exporter::trace::OStreamSpanExporterFactory::Create();
    auto processor = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));

    // ***
    auto provider = opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(processor), resource);
    // ***

    const opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider> api_provider(provider.release());
    opentelemetry::trace::Provider::SetTracerProvider(api_provider);

    // Your code goes here

    return 0;
}
