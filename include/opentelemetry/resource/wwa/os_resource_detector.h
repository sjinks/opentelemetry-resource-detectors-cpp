#ifndef AB6F045F_7E1D_4233_ADE1_D44A4AB8DB63
#define AB6F045F_7E1D_4233_ADE1_D44A4AB8DB63

#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>

#include "export.h"

namespace wwa::opentelemetry::resource {

class WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT os_resource_detector
    : public ::opentelemetry::sdk::resource::ResourceDetector {
public:
    ::opentelemetry::sdk::resource::Resource Detect() override;
};

}  // namespace wwa::opentelemetry::resource

#endif /* AB6F045F_7E1D_4233_ADE1_D44A4AB8DB63 */
