%module glext
%insert("fsiinclude")
%{
/* #define __wchar_t__ 42 */
#include <GL/gl.h>
#include <GL/glext.h>
%}

/* prevent syntax error in stddef.h */
/* #define __wchar_t__ 42 */
#define GL_GLEXT_PROTOTYPES 1
%include <GL/glew.h>
%include <GL/glext.h>
%include <GL/gl.h>
