// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "shader.h"

#include <stddef.h>
#include <stdint.h>

#define NAM_S(name) _binary_res_##name##_start // name of a start variable
#define NAM_E(name) _binary_res_##name##_end   // name of an end variable

// NOTE: we are currently just sucking up the memory costs for ease. We can either include the source files themselves. Or use compression, where I'd prefer the latter for ease of installation.
// NOLINTBEGIN (bugprone-reserved-identifier)
extern char const NAM_S(sh_vert_glsl)[];
extern char const NAM_E(sh_vert_glsl)[];
extern char const NAM_S(sh_frag_glsl)[];
extern char const NAM_E(sh_frag_glsl)[];
// NOLINTEND

/* compile a shader */
static GLuint shader_compile(GLenum type, char const* src, size_t len) {
	int ilen = len;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, &ilen);
	glCompileShader(shader);
	return shader;
}

#define COMPILE_NAME(_type, _name) shader_compile(_type, NAM_S(_name), NAM_E(_name) - NAM_S(_name))
int shader_init(GLuint pipe) {
	GLuint vs = COMPILE_NAME(GL_VERTEX_SHADER, sh_vert_glsl);
	GLuint fs = COMPILE_NAME(GL_FRAGMENT_SHADER, sh_frag_glsl);

	glAttachShader(pipe, vs);
	glAttachShader(pipe, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return 1;
}
