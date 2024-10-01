#ifndef FC650560_3B09_4CB5_9C31_1BA850805765
#define FC650560_3B09_4CB5_9C31_1BA850805765

#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/sdk/resource/resource_detector.h>

#include "export.h"

namespace wwa::opentelemetry::resource {

class WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT process_resource_detector
    : public ::opentelemetry::sdk::resource::ResourceDetector {
public:
    ::opentelemetry::sdk::resource::Resource Detect() override;
};

}  // namespace wwa::opentelemetry::resource

#endif /* FC650560_3B09_4CB5_9C31_1BA850805765 */
