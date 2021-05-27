#version 330
uniform mat4                modelViewMatrix;
uniform mat4                modelViewProjMatrix;
uniform float               ptRadius;
uniform float               ptScale;
in vec4                     vertex;
out vec3                    eyePos;

void main(void)
{
    //Calculate window-space point size
    eyePos = vec3(modelViewMatrix * vec4(vertex.xyz, 1.0));
    float distance = length(eyePos);
    gl_PointSize = ptRadius * (ptScale / distance);
    gl_Position = modelViewProjMatrix * vertex;
}
