#version 330
uniform sampler2D   textureSampler;
in mediump vec4     texCoord;
out mediump vec4    color;

void main(void)
{
    color = texture(textureSampler, texCoord.st);
}
