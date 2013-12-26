#define NUM_LIGHTS 4

uniform mat4 model, view, proj;
uniform mat4 lights[NUM_LIGHTS];
uniform vec3 ka, kd, ks;
uniform float ns, tr;
uniform bool show_backface;

in vec3 vPos;
in vec3 vNormal;
in vec2 vertexUV;

varying vec3 fPos;
varying vec3 ambientAndDiffuse, specular;
varying vec2 fUv;
varying float depth;

void main() {
    mat4 modelView = view * model;

    fPos = (modelView * vec4(vPos, 1.f)).xyz;

    ambientAndDiffuse = vec3(0);
    specular = vec3(0);
    vec3 N = normalize((modelView * vec4(vNormal, 0.f)).xyz);
    vec3 E = normalize(-fPos);

    for (int i = 0; i < NUM_LIGHTS; ++i) {

        vec3 L = normalize((view * vec4(lights[i][3].xyz, 1.f)).xyz - fPos);
        vec3 H = normalize(L + E);

        ambientAndDiffuse += lights[i][0].xyz * ka;
        ambientAndDiffuse += lights[i][1].xyz * kd * max(dot(L, N), 0.f);
        specular += lights[i][2].xyz * ks * pow(max(dot(N, H), 0.f), ns)
                  * (dot(L, N) < 0.f ? 0.f : 1.f);
    }

    fUv = vertexUV.xy;

    gl_Position = proj * vec4(fPos, 1);

    depth = length(gl_Position);
    depth *= depth;
}
