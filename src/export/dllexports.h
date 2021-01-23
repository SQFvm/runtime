#pragma once
#include <vector>


#ifdef _WIN32
#define DLLEXPORT_PREFIX __declspec(dllexport)
#else
#define DLLEXPORT_PREFIX __attribute__((visibility("default")))
#endif

extern "C" {
    // A callback for logging
    // @param user_data Data set at creating the instance
    // @param call_data Data set during call
    // @param severity Enum vlaue with the following Possibilities:
    //                -1: RESULT
    //                 0: fatal
    //                 1: error
    //                 2: warning
    //                 3: info
    //                 4: verbose
    //                 5: trace
    typedef void(*sqfvm_log_callback)(void* user_data, void* call_data, int32_t severity, const char* message, uint32_t length);

    // Creates a new SQF-VM instance
    // @param callback The callback to report log messages on
    // @param user_data Custom data from the callee that will be passed into callback
    // @param max_runtime_seconds the maximum runtime in seconds as float. Pass 0
    // Returns NULL on error
    void* sqfvm_create_instance(void* user_data, sqfvm_log_callback callback, float max_runtime_seconds);

    // Destroys a previously created SQF-VM instance
    void sqfvm_destroy_instance(void* instance);

    // Loads the provided config into the config tree of the provided instance
    // @param instance A valid instance as returned from sqfvm_create_instance
    // @param contents The config contents to load
    // @param length The length of contents.
    // @return  0 if loading the config was successful,
    //         -1 if the instance was null
    //         -2 if preprocessing failed
    //         -3 if parsing failed
    int32_t sqfvm_load_config(void* instance, const char* contents, uint32_t length);

    // Checks the status of the instance
    // @param instance A valid instance as returned from sqfvm_create_instance
    // @return -1 if the instance was null
    //          0 Instance is not running code and currently ready for usage
    //         +1 Instance is currently in HALTED mode
    //         +2 Instance is currently running
    //         +3 Instance is in halted_error
    //         +4 evaluating
    //         
    int32_t sqfvm_status(void* instance);

    // Calls the provided code, using the SQF-VM instance.
    // @param instance A valid instance as returned from sqfvm_create_instance
    // @param call_data data that will be passed into the callback for logs of the given instance
    // @param type The type of code to execute.
    //             Can be one of the following values:
    //             - 'a' denotes assembly code
    //             - 's' denotes SQF code
    //             - 'c' denotes SQC code
    //             - 'p' denotes that the code provided should only be ran through the preprocessor
    // @param code The code to execute
    // @param length The length of the code to execute.
    // @return  0 if calling the SQF-VM instance was successful,
    //         -1 if the instance was null
    //         -2 if preprocessing failed
    //         -3 if parsing failed
    //         -4 if the instance is already running
    //         -5 if the provided type was invalid
    //         -6 if the execution did not succeed
    int32_t sqfvm_call(void* instance, void* call_data, char type, const char* code, uint32_t length);
}