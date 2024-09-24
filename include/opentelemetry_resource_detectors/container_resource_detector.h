#ifndef B972DDC2_4D7D_4496_8504_A4AA5C23AD2B
#define B972DDC2_4D7D_4496_8504_A4AA5C23AD2B

#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>

#include "opentelemetry_resource_detectors_export.h"

namespace wwa::opentelemetry {

class OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT container_resource_detector
    : public ::opentelemetry::sdk::resource::ResourceDetector {
public:
    ::opentelemetry::sdk::resource::Resource Detect() override;
};

}  // namespace wwa::opentelemetry

#endif /* B972DDC2_4D7D_4496_8504_A4AA5C23AD2B */
