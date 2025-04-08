#include "platform.h"

#if defined(TARGET_NDS)
#include "nds/platform_impl.c"
#elif defined(TARGET_DESKTOP)
#include "desktop/platform_impl.c"
#else
#error "No valid platform defined"
#endif

