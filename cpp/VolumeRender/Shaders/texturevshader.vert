#version 330
uniform mat4        modelViewProjMatrix;
in highp vec4       vertex;
in mediump vec4     texCoordIn;
out mediump vec4    texCoord;

void main(void)
{
    gl_Position = modelViewProjMatrix * vertex;
    texCoord = texCoordIn;
}
