//OS Specific.
//Don't know if _MAC_ or _WINDOWS will be used, but added them anyway.
#if defined(__APPLE__) || defined(MACOSX)
#define _MAC_
#include <cyclone/cyclone.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define _WINDOWS_
#include <cyclone\cyclone.h>
#endif

//Other util stuff.
#include "app.h"
#include "ogl_headers.h"
#include "timing.h"