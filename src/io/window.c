// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#define GLAD_GL_IMPLEMENTATION
#include "window.h"

#include <glad/gl.h>
#include <stdio.h>
#include <string.h>

#include "../error.h"
#include "../util/vec/float2.h"

// include system libraries
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "input.h"

// macros for ease of access
#define WIN_NAME           "MCA Selector Lite"
#define WIN_DEFAULT_WIDTH  640
#define WIN_DEFAULT_HEIGHT 480

static GLFWwindow* win = NULL;

int window_init(void) {
#ifndef NDEBUG
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

	// initialize the window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // sets the profile to "core", so old, deprecated functions are disabled.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	win = glfwCreateWindow(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, WIN_NAME, NULL, NULL);
	if (!win) return 1;

	// setup OpenGL
	glfwMakeContextCurrent(win);
	if (!gladLoadGL(glfwGetProcAddress)) return 1;

	// configure callbacks
	glfwSetKeyCallback(win, key_callback);
	glfwSwapInterval(1); // wait 1 screen update for a redraw a.k.a. "vsync". (not really applicable in this case but eh)

	// print the OpenGL version information
	debug("version info:");
	debug("\tvendor:       %s", glGetString(GL_VENDOR));
	debug("\trenderer:     %s", glGetString(GL_RENDERER));
	debug("\tversion:      %s", glGetString(GL_VERSION));
	debug("\tshading lang: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return 0;
}

/* initialize a shader */
static uint32_t init_shader(GLenum type, char const* src) {
	uint32_t shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	return shader;
}

/* initialize the graphics pipeline */
static uint32_t init_pipe(void) {
	// create the graphics pipeline context
	uint32_t pipe = glCreateProgram();

	uint32_t vs = init_shader(GL_VERTEX_SHADER, "#version 330 core\nin vec4 pos; void main() { gl_Position = vec4(pos.x, pos.y, pos.z, pos.w); }");
	uint32_t fs = init_shader(GL_FRAGMENT_SHADER, "#version 330 core\nout vec4 colour; void main() { colour = vec4(1.0F, 0.5F, 0.0F, 0.1F); }");

	glAttachShader(pipe, vs);
	glAttachShader(pipe, fs);
	glLinkProgram(pipe);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glValidateProgram(pipe);

	return pipe;
}

void window_loop(void) {
	assert(win != NULL);

	uint32_t pipe = init_pipe();

	unsigned vertc = 6;
	float2 vert[] = {
		{-1, -1   },
		{1,  -1   },
		{1,  -0.9F},

		{-1, -0.9F},
		{1,  -0.9F},
		{-1, -1   },
	};

	uint32_t vbo; // vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	GLuint vao; // vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float2), NULL);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(win)) {
		glfwWaitEvents(); // wait till an update has been given

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		int w, h;
		glfwGetWindowSize(win, &w, &h);
		glViewport(0, 0, w, h);
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glUseProgram(pipe);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertc);

		glfwSwapBuffers(win);
	}

	glfwDestroyWindow(win);
}
