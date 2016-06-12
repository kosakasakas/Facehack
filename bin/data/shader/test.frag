#version 410

in vec3 o_normal;
in vec2 o_texcoord;

out vec4 outputColor;

uniform sampler2D u_sampleTex;

void main()
{
    vec3 col    = texture(u_sampleTex, o_texcoord).rgb;
    outputColor = vec4(col.rgb,1.0);
}

