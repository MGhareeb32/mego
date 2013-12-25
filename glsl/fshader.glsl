#define NUM_LIGHTS 4
#define LOG2 1.442695

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

varying mat4 modelView;
varying vec3 fPos;
varying vec3 fN, fE;
varying vec2 fUv;

uniform sampler2D myTextureSampler;

vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.f, -1.f / 3.f, 2.0 / 3.f, -1.f);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.f * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.f, 2.f / 3.f, 1.f / 3.f, 3.f);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.f - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.f, 1.f), c.y);
}

void main() {
    // TEXTURES
    vec4 tex = texture_flag ?
        texture2D(myTextureSampler, fUv) : vec4(1.f);

    // LIGHT
    vec3 afterLight[NUM_LIGHTS];
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    if (!show_backface && dot(N, E) < 0.f)
        discard;
    for (int i = 0; i < NUM_LIGHTS; ++i) {

        vec3 L = normalize((view * vec4(lights[i][3].xyz, 1.f)).xyz - fPos);
        vec3 H = normalize(L + E);

        vec3 ambient = lights[i][0].xyz * ka;
        vec3 diffuse = lights[i][1].xyz * kd * max(dot(L, N), 0.f);
        vec3 specular = lights[i][2].xyz * ks * pow(max(dot(N, H), 0.f), ns);
        
        specular *= (dot(L, N) < 0.f ? 0.f : 1.f);

        afterLight[i] = (ambient + diffuse) * tex.xyz + specular;
    }
    vec3 finalAfterLight = scene_color;
    for (int i = 0; i < NUM_LIGHTS; ++i)
        finalAfterLight += afterLight[i];

    // FOG
    float depth2 = length(proj * vec4(fPos, 1.f));
    depth2 *= depth2;
    vec3 afterFog = mix(finalAfterLight, scene_fog_color,
                        clamp(depth2 * scene_fog_mag, 0.f, 1.f));

    gl_FragColor = vec4(afterFog, tr * tex.w * blend_color.w);
}
