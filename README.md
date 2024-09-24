# opentelemetry-resource-detectors-cpp

[![Build and Test](https://github.com/sjinks/opentelemetry-resource-detectors-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/sjinks/opentelemetry-resource-detectors-cpp/actions/workflows/ci.yml)
[![CodeQL](https://github.com/sjinks/opentelemetry-resource-detectors-cpp/actions/workflows/codeql.yml/badge.svg)](https://github.com/sjinks/opentelemetry-resource-detectors-cpp/actions/workflows/codeql.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=sjinks_opentelemetry-resource-detectors-cpp&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=sjinks_opentelemetry-resource-detectors-cpp)

Resource detectors for OpenTelemetry (C++).

The library provides the following resource detectors:
* [container](https://opentelemetry.io/docs/specs/semconv/resource/container/) (`container.id`);
* [process](https://opentelemetry.io/docs/specs/semconv/resource/process/) (`process.command_line`, `process.group_leader.pid`, `process.interactive`, `process.parent_pid`, `process.pid`, `process.real_user.id`, `process.real_user.name`, `process.saved_user.id`, `process.saved_user.name`, `process.session_leader.pid`, `process.user.id`, `process.user.name`);
* [OS](https://opentelemetry.io/docs/specs/semconv/resource/os/) (`os.type`, `os.version`) and [host](https://opentelemetry.io/docs/specs/semconv/resource/host/) (`host.arch`, `host.name`).

## Installation

```shell
cmake -B build
cmake --build build
cmake --install build
```

**Dependencies:** [opentelemetry-cpp](https://github.com/open-telemetry/opentelemetry-cpp)

**Development Dependencies:** [Google Test](https://github.com/google/googletest) (required to build and run tests), [JSON for Modern C++](https://github.com/nlohmann/json) (required to build examples)

### CMake Options

| Option Name                  | Description                      | Default Value |
|------------------------------|----------------------------------|---------------|
| `WITH_TESTING`               | Whether to enable tests          | `ON`          |
| `WITH_EXAMPLES`              | Whether to build examples        | `ON`          |
| `INSTALL_RESOURCE_DETECTORS` | Whether to install the library   | `ON`          |

## Usage

```cmake
find_package(opentelemetry_resource_detectors CONFIG REQUIRED)
target_link_libraries(
    my_target
    PRIVATE
        opentelemetry_resource_detectors::opentelemetry_resource_detectors
)
```

```cpp
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

#include <opentelemetry_resource_detectors/container_resource_detector.h>
#include <opentelemetry_resource_detectors/os_resource_detector.h>
#include <opentelemetry_resource_detectors/process_resource_detector.h>

int main()
{
    std::vector<std::unique_ptr<opentelemetry::sdk::resource::ResourceDetector>> detectors;
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::container_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::os_resource_detector>());
    detectors.emplace_back(std::make_unique<wwa::opentelemetry::process_resource_detector>());

    auto resource = opentelemetry::sdk::resource::Resource::Create({});

    for (const auto& detector : detectors) {
        auto detected = detector->Detect();
        resource      = resource.Merge(detected);
    }

    // Now that the resource is ready, pass it to providers

    auto exporter  = opentelemetry::exporter::trace::OStreamSpanExporterFactory::Create();
    auto processor = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(
        std::move(exporter)
    );

    std::unique_ptr<opentelemetry::sdk::trace::TracerProvider> provider =
        opentelemetry::sdk::trace::TracerProviderFactory::Create(
            std::move(processor),
            resource               // <===
        );

    const opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider> api_provider(
        provider.release()
    );

    opentelemetry::trace::Provider::SetTracerProvider(api_provider);

    // Your code goes here
}
```

**Note:** unlike the OpenTelemetry JS SDK, the C++ SDK merges resources differently. In the JS SDK, `Resource.Merge()` overwrites old attributes. In the C++ SDK, it preserves old attributes.
