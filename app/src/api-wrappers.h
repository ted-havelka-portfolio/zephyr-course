#ifndef TASKS_APP_API_WRAPPERS
#define TASKS_APP_API_WRAPPERS

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t wrapper_show_settings(void);

int32_t wrapper_set_id(const uint32_t id);

#ifdef __cplusplus
}
#endif

#endif // TASKS_APP_API_WRAPPERS
