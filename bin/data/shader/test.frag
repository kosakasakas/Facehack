#version 410

in vec3 o_normal;
in vec2 o_texcoord;
in vec4 o_color;

out vec4 outputColor;

void main()
{
    vec3  viewVec = vec3(0.0, 0.0, 1.0);
    float lambert = min(max(dot(viewVec, o_normal), 0.0), 1.0);
    outputColor = vec4(lambert * o_color.rgb, 1.0);
}

