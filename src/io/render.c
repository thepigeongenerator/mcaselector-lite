/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "render.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdint.h>

#include "../error.h"
#include "../util/intdef.h"
#include "shader.h"

#define VERTC 3
static GLuint pipe;
static GLuint vbo;        // vertex buffer object
static GLuint vao;        // vertex array object
static GLuint screen_loc; // location to where OpenGL sends to the shaders of the screen dimensions
static int win_w, win_h;

static void screen_resize(int w, int h) {
	i32 verts[VERTC][4] = {
		{0, 0,  w, 20    },
		{0, 20, w, h - 40},
		{0, h,  w, -20   },
	};

	glUniform2i(screen_loc, w, h);                                        // send the screen dimensions to the shader pipeline
	glViewport(0, 0, w, h);                                               // update the viewport
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW); // bind the data to it
	win_w = w;
	win_h = h;
}

int render_init(void) {
	pipe = glCreateProgram();
	shader_init(pipe);       // initialise and include the shaders
	glLinkProgram(pipe);     // link the application
	glValidateProgram(pipe); // validate that what we've done is correct

	int len;
	glGetProgramiv(pipe, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		char log[len];
		glGetProgramInfoLog(pipe, len, &len, log);
		log[len - 1] = '\0'; // terminate the string one character sooner since the log includes a newline
		fatal("error whilst linking the pipe: '%s'", log);
	}

	screen_loc = glGetUniformLocation(pipe, "screen");

	glGenBuffers(1, &vbo);              // create the vertex buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind to it

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set VBO info
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0); // set the array data index to 0
	glVertexAttribIPointer(0, 4, GL_INT, 4 * sizeof(i32), NULL);

	glBindVertexArray(0);
	return 0;
}

void render_update(GLFWwindow *win) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w, h;
	glfwGetWindowSize(win, &w, &h);
	glUseProgram(pipe);

	if (w != win_w || h != win_h)
		screen_resize(w, h);

	glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, VERTC);
}

void render_free(void) {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(pipe);
	vbo = 0;
	vao = 0;
	pipe = 0;
}
