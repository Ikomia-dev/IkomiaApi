#version 330
uniform mat4    projMatrix;
uniform float   ptRadius;   // point size in world space
uniform vec4    color;
in vec3         eyePos;     // position of center in eye space
out vec4        colorOut;

void main(void)
{
    const vec3 lightDir = vec3(0.0, 0.0, 1.0);
    const float shininess = 16.0;

    //Calculate normal from texture coordinates
    vec3 n;
    n.xy = gl_PointCoord.xy * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(n.xy, n.xy);

    if (mag > 1.0)
        discard;   // kill pixels outside circle

    n.z = sqrt(1.0 - mag);

    //Point on surface of sphere in eye space
    vec3 spherePosEye = eyePos + (n * ptRadius);

    //Calculate lighting
    float diffuse = max(0.0, dot(lightDir, n));
    vec3 v = normalize(-spherePosEye);
    vec3 h = normalize(lightDir + v);
    float specular = pow(max(0.0, dot(n, h)), shininess);

    //Compute color
    colorOut.xyz = color.xyz * diffuse + specular;
    colorOut.a = color.a;

    //Extracting the z component
    vec4 projectedPt = projMatrix * vec4(spherePosEye, 1.0);
    projectedPt /= projectedPt.w;
    gl_FragDepth = projectedPt.z * 0.5 + 0.5;
}
