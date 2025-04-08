#include "platform.h"

// Determines which code file to include, depending on which target platform was specified in the build process

#if defined(TARGET_NDS)
#include "nds/platform_impl.c"
#elif defined(TARGET_WII)
#include "wii/platform_impl.c"
#elif defined(TARGET_DESKTOP)
#include "desktop/platform_impl.c"
#else
#error "No platform defined"
#endif

