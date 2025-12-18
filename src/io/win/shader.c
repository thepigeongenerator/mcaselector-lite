/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#include "shader.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../../util/error.h"


// NOTE: we are currently just sucking up the memory costs for ease. We can either include the source files themselves. Or use compression, where I'd prefer the latter for ease of installation.
extern const char sh_vert_glsl[];
extern const char sh_frag_glsl[];
extern const char sh_geom_glsl[];
extern const uint sh_vert_glsl_len;
extern const uint sh_frag_glsl_len;
extern const uint sh_geom_glsl_len;

/* Compiles a shader of `type` from `src` with `len` bytes.
 * Returns the integer for the shader. */
static GLuint shader_compile(GLenum type, const char *src, usize len)
{
	int    ilen   = len;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, &ilen);
	glCompileShader(shader);

	// repurposing ilen for the max length of the shader log
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &ilen);
	if (ilen > 0) {
		char log[ilen];
		glGetShaderInfoLog(shader, ilen, &ilen, log);
		log[ilen - 1] = '\0'; // terminate the string one character sooner since the log includes a newline
		error("error whilst compiling shader type '0x%X': '%s'", type, log);
	}

	return shader;
}

int shader_init(GLuint pipe)
{
	GLuint vs = shader_compile(GL_VERTEX_SHADER, sh_vert_glsl, sh_vert_glsl_len);
	GLuint fs = shader_compile(GL_FRAGMENT_SHADER, sh_frag_glsl, sh_frag_glsl_len);
	GLuint gs = shader_compile(GL_GEOMETRY_SHADER, sh_geom_glsl, sh_geom_glsl_len);

	glAttachShader(pipe, vs);
	glAttachShader(pipe, fs);
	glAttachShader(pipe, gs);

	// mark shaders off for deletion
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
	return 1;
}
