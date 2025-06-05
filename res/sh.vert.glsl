#version 330 core

layout(location = 0) in vec4 pos;

void main() {
	gl_Position = vec4(pos.x, pos.y, 0.0F, 1.0F);
}
