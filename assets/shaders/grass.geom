#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 TexCoord[];

out vec2 gsTexCoord;

void main() {
    // Iterate over the three vertices of the input triangle
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;

        // Example: Offset the position to create the appearance of grass blades
        gl_Position.y += 0.1 * sin(gl_Position.x * 5.0) * sin(gl_Position.z * 5.0);

        gsTexCoord = TexCoord[i];
        EmitVertex();
    }
EndPrimitive();
}
