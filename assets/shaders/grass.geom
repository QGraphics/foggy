#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

in vec2 TexCoord[];

out vec2 gsTexCoord;

void main() {
    // Iterate over the three vertices of the input triangle

    for (int i = 0; i < 3; ++i) {
        // Emit the base vertex for the original triangle
        gl_Position = gl_in[i].gl_Position;
        gsTexCoord = TexCoord[i];
        EmitVertex();
    }

    for (int i = 0; i < 3; ++i) {
        // Emit the base vertex
        gl_Position = gl_in[i].gl_Position;
        gsTexCoord = TexCoord[i];
        EmitVertex();

        // Emit an additional vertex above the current one
        gl_Position.y += 0.3;  // Adjust the height as needed
        gsTexCoord = TexCoord[i] + vec2(0.0, 0.1);  // Adjust texture coordinates for grass texture
        EmitVertex();

        // Emit an additional vertex above the current one and shifted to form a triangle
        gl_Position.y += 0.3;  // Adjust the height as needed
        gl_Position.x += 0.1;  // Adjust the x-component to shift
        gsTexCoord = TexCoord[i] + vec2(0.1, 0.1);  // Adjust texture coordinates for grass texture
        EmitVertex();

        // Emit an additional vertex below the current one
        gl_Position.y = gl_in[i].gl_Position.y;  // Reset the y-component
        gl_Position.x += 0.1;  // Adjust the x-component to shift
        gsTexCoord = TexCoord[i] + vec2(0.1, 0.0);  // Adjust texture coordinates for grass texture
        EmitVertex();
    }

    EndPrimitive();
}