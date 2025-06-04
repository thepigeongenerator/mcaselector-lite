// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "render.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdlib.h>

#include "../util/vec/float2.h"
#include "shader.h"

#define VERTC 6
GLuint pipe;
float2 verts[VERTC] = {
	{-1, -1   }, // pnt A
	{1,  -1   }, // pnt B
	{1,  -0.9F}, // pnt C

	{-1, -0.9F}, // pnt D
	{1,  -0.9F}, // pnt C
	{-1, -1   }, // pnt A
};
GLuint vbo; // vertex buffer object
GLuint vao; // vertex array object

int render_init(void) {
	pipe = glCreateProgram();
	shader_init(pipe);       // initialise and include the shaders
	glLinkProgram(pipe);     // link the application
	glValidateProgram(pipe); // validate that what we've done is correct

	// init the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// init the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// set VBO info
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float2), NULL);

	glBindVertexArray(0);
	return 0;
}

void render_update(GLFWwindow* win) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w, h;
	glfwGetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(pipe);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, VERTC);
}
