#version 330 core

#define NUM_LIGHTS 32
#define LOG2 1.442695
precision mediump float;

uniform bool show_backface;

uniform mat4 model, view, proj;

uniform float scene_fog_mag;
uniform vec3 scene_fog_color, scene_color;

uniform mat4 lights[NUM_LIGHTS];

uniform vec3 ka, kd, ks;
uniform float ns, tr;
uniform bool texture_flag;

uniform vec4 blend_color;
uniform vec4 blend_factor;

in mat4 modelView;
in vec3 fPos;
in vec3 fN, fE;
in vec2 fUv;

uniform sampler2D myTextureSampler;

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    // TEXTURES
    vec4 tex = texture_flag ?
        texture2D(myTextureSampler, fUv) : vec4(1);

    // LIGHT
    vec3 afterLight[NUM_LIGHTS];
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
//    if (!show_backface && dot(N, E) < 0)
//        discard;
    for (int i = 0; i < NUM_LIGHTS; ++i) {

        vec3 L = normalize((view * vec4(lights[i][3].xyz, 1)).xyz - fPos);
        vec3 H = normalize(L + E);

        vec3 ambient = lights[i][0].xyz * ka;
        vec3 diffuse = lights[i][1].xyz * kd * max(dot(L, N), 0);
        vec3 specular = lights[i][2].xyz * ks * pow(max(dot(N, H), 0), ns);
        
        specular *= (dot(L, N) < 0 ? 0.f : 1.f);

        afterLight[i] = (ambient + diffuse) * tex.xyz + specular;
    }
    vec3 finalAfterLight = scene_color;
    for (int i = 0; i < NUM_LIGHTS; ++i)
        finalAfterLight += afterLight[i];

    // FOG
    float depth2 = length(proj * vec4(fPos, 1));
    depth2 *= depth2;
    vec3 afterFog = mix(finalAfterLight, scene_fog_color,
                        clamp(depth2 * scene_fog_mag, 0, 1));

    gl_FragColor = vec4(afterFog, tr * tex.w * blend_color.w);
}
