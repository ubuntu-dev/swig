%module opengl
%insert("fsiinclude")
%{
/* #define __wchar_t__ 42 */
#include <GL/gl.h>
%}

/* prevent syntax error in stddef.h */
/* #define __wchar_t__ 42 */

%include <GL/gl.h>
