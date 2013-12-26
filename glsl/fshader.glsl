#define NUM_LIGHTS 4

uniform mat4 model, view, proj;

uniform float scene_fog_mag;
uniform vec3 scene_fog_color, scene_color;

uniform float tr;
uniform bool show_backface;
uniform bool texture_flag;

varying vec3 fPos;
varying vec3 ambientAndDiffuse, specular;
varying vec2 fUv;
varying float depth;

uniform sampler2D myTextureSampler;


void main() {
    // TEXTURES
    vec4 tex = texture_flag ?
        texture2D(myTextureSampler, fUv) : vec4(1.f);

    // LIGHT
    vec3 finalAfterLight = scene_color
                         + tex.xyz * (ambientAndDiffuse)
                         + specular;

    // FOG
    vec3 afterFog = mix(finalAfterLight, scene_fog_color,
                        clamp(depth * scene_fog_mag, 0.f, 1.f));

    gl_FragColor = vec4(afterFog, tr * tex.w);
}
