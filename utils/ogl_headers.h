#ifndef _OPENGL_HEADERS_
#define _OPENGL_HEADERS_

#include <stdlib.h>

#ifdef __gnu_linux__
    #include <GL/glut.h>
#elif defined(__APPLE__) || defined(MACOSX)
    #include <GLUT/glut.h>
#else
    #include <gl/glut.h>
#endif

#endif // _OPENGL_HEADERS_