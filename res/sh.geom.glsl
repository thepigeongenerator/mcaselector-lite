#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out; // output the vertices of this rectangle

void push_vert(float x, float y) {
	gl_Position = vec4(x, y, 0.0F, 1.0F);
	EmitVertex();
}

void main() {
	// position data is stored as (x,y,w,h)
	vec4 rect_a = gl_in[0].gl_Position; // get the rectangle data which will be our first point of the rectangle
	vec2 rect_d = vec2(                 // compute point D
		rect_a.x + rect_a.z,
		rect_a.y - rect_a.w);

	// A---B
	// C---D
	// triangle_strip will use the last two vertices to create a triangle
	push_vert(rect_a.x, rect_a.y); // A
	push_vert(rect_d.x, rect_a.y); // B
	push_vert(rect_a.x, rect_d.y); // C
	push_vert(rect_d.x, rect_d.y); // D
	EndPrimitive();
}
