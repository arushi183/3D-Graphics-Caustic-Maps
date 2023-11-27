#version 330 core

in vec2 frag_uv;

// Assuming these are coming from the vertex shader
in vec4 worldPosition;
in float depth;

out vec4 fragColor;

void main() {
    fragColor = vec4(worldPosition.xyz, depth, 1.0);
}