#ifndef BAA5D07F_C329_4EC2_9E63_19F4CBB256C8
#define BAA5D07F_C329_4EC2_9E63_19F4CBB256C8

#ifdef WWA_OPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE
#    define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT
#    define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_NO_EXPORT
#else
#    ifdef wwa_opentelemetry_resource_detectors_EXPORTS
/* We are building this library; export */
#        if defined _WIN32 || defined __CYGWIN__
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT __declspec(dllexport)
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_NO_EXPORT
#        else
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT    [[gnu::visibility("default")]]
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    else
/* We are using this library; import */
#        if defined _WIN32 || defined __CYGWIN__
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT __declspec(dllimport)
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_NO_EXPORT
#        else
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_EXPORT    [[gnu::visibility("default")]]
#            define WWA_OPENTELEMETRY_RESOURCE_DETECTORS_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    endif
#endif

#endif /* BAA5D07F_C329_4EC2_9E63_19F4CBB256C8 */
