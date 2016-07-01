#version 410
#
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 o_normal;
out vec2 o_texcoord;
out vec4 o_color;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;
    mat3 mv3    = mat3(modelViewMatrix);
    mat3 NMat   = transpose(inverse(mv3)); 
    o_normal    = NMat * normal;
    o_texcoord  = texcoord;
    o_color     = color;
}

