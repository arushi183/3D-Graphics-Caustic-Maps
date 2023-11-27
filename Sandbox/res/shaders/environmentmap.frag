#version 330 core

in vec2 frag_uv;
int float gl_FragDepth

// Assuming these are coming from the vertex shader
in vec4 worldPosition;
//in float depth;

out vec4 fragColor;

void main() {
    fragColor = vec4(worldPosition.xyz, gl_FragDepth);
}