// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "shader.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../error.h"

#define NAM_S(name) _binary_res_##name##_start // name of a start variable
#define NAM_E(name) _binary_res_##name##_end   // name of an end variable

// macro for generating the variable declarations
#define DEF_GLSL(name)                   \
	extern char const NAM_S(name)[]; \
	extern char const NAM_E(name)[]

// NOTE: we are currently just sucking up the memory costs for ease. We can either include the source files themselves. Or use compression, where I'd prefer the latter for ease of installation.
// NOLINTBEGIN (bugprone-reserved-identifier)
DEF_GLSL(sh_vert_glsl);
DEF_GLSL(sh_frag_glsl);
DEF_GLSL(sh_geom_glsl);
// NOLINTEND

/* compile a shader */
static GLuint shader_compile(GLenum type, const char *src, size_t len) {
	int ilen = len;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, &ilen);
	glCompileShader(shader);

	// repurposing ilen for the max length of the shader log
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &ilen);
	if (ilen > 0) {
		char log[ilen];
		glGetShaderInfoLog(shader, ilen, &ilen, log);
		log[ilen - 1] = '\0'; // terminate the string one character sooner since the log includes a newline
		error("error whilst compiling shader type '0X%X': '%s'", type, log);
	}

	return shader;
}

#define COMPILE_NAME(_type, _name) shader_compile(_type, NAM_S(_name), (uintptr_t)NAM_E(_name) - (uintptr_t)NAM_S(_name))
int shader_init(GLuint pipe) {
	GLuint vs = COMPILE_NAME(GL_VERTEX_SHADER, sh_vert_glsl);
	GLuint fs = COMPILE_NAME(GL_FRAGMENT_SHADER, sh_frag_glsl);
	GLuint gs = COMPILE_NAME(GL_GEOMETRY_SHADER, sh_geom_glsl);

	glAttachShader(pipe, vs);
	glAttachShader(pipe, fs);
	glAttachShader(pipe, gs);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
	return 1;
}
