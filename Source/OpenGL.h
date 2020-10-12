#ifndef __OPEN_GL_H__
#define __OPEN_GL_H__

#define WIN32_MEAN_AND_LEAN							// Reduces the size of the Win32 header files, which speeds up the build process.
#include <Windows.h>
#include "Dependencies/Glew/include/GL/glew.h"			// Extension library for OpenGL. Must be included before any other OpenGL include.
#include "Dependencies/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#endif // !__OPEN_GL_H__