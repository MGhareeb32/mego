#version 330 core

#define M_PI 3.1415926535897932384626433832795

uniform mat4 model, view, proj;

in vec3 vPos;
in vec3 vNormal;
in vec2 vertexUV;

out mat4 modelView;
out vec3 fPos;
out vec3 fN, fE;
out vec2 fUv;

void main() {
    modelView = view * model;

    fPos = (modelView * vec4(vPos, 1.f)).xyz;
    fE = -fPos;

    fN = (modelView * vec4(vNormal, 0.f)).xyz;

    fUv = vertexUV.xy;

    gl_Position = proj * vec4(fPos, 1);
}
