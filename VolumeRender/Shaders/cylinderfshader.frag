#version 330
uniform mat4    modelViewMatrix;
uniform mat4    modelViewMatrixInverse;
uniform mat4    projMatrix;
uniform vec4    color;
uniform float   radius;

in vec3         U;
in vec3         V;
in vec3         H;
in float        cylinderLength;
in vec4         vertexBase;
in vec3         localCoord;

//out vec4        colorOut;
layout(location=0) out vec4 colorOut;
layout(location=1) out vec4 normalOut;

vec3 phong_shading(vec3 lightDir, vec3 cameraDir, vec3 normal, vec3 color)
{
    vec3 halfvector = normalize(lightDir + cameraDir);

    float NdotL = abs(dot(normalize(lightDir), normal));
    float NdotHV = max(dot(normal, halfvector), 0.0f);

    //Shininess
    float specular = 1.0f * pow(NdotHV, 15.0f);
    return color * (0.8 * NdotL) + specular;
}

vec3 toon_shading(vec3 lightDir, vec3 normal, vec3 color)
{
    float NdotL = dot(normalize(lightDir), normal);
    vec3 ret;

    if (NdotL <= 0.4)
        ret = vec3(0.0, 0.0, 0.0);
    else if (NdotL <= 0.6)
        ret = 0.75 * color;
    else
        ret = color;

    return ret;
}

void main()
{
    const int shading = 0;

    //Cylinder base in wiew space
    vec4 origin = modelViewMatrix * vertexBase;
    origin /= origin.w;

    //First of all, I need the correct point that we're pointing at
    vec3 surfacePt = origin.xyz;
    surfacePt.xyz += U * (localCoord.x*2.0 - 1.0) * radius;
    surfacePt.xyz += V * (localCoord.y*2.0 - 1.0) * radius;
    surfacePt.xyz += H * localCoord.z * cylinderLength;

    //We need to raytrace the cylinder (!)
    //we can do the intersection in impostor space
    //Calculate the ray direction in viewspace
    vec3 cameraPos = vec3(vec4(0.0, 0.0, 0.0, 1.0) * modelViewMatrixInverse);
    vec3 rayOrigin = cameraPos;
    vec3 rayTarget = surfacePt.xyz;
    vec3 rayDirection = normalize(rayTarget - rayOrigin);

    //Basis = local coordinate system of cylinder
    mat3 basis = transpose(mat3(U, V, H));
    vec3 base = origin.xyz;
    vec3 endPt = origin.xyz + H * cylinderLength;

    //Origin of the ray in cylinder space
    vec3 P = -origin.xyz;
    P = basis * P;

    //Direction of the ray in cylinder space
    vec3 D = basis * rayDirection;

    //Now the intersection is between z-axis aligned cylinder and a ray
    float c = P.x*P.x + P.y*P.y - radius*radius;
    float b = 2.0*(P.x*D.x + P.y*D.y);
    float a = D.x*D.x + D.y*D.y;
    float d = b*b - 4*a*c;

    if (d < 0.0)
        discard;

    float t1 = (-b - sqrt(d))/(2*a);
    vec3 newPt1 = rayOrigin + t1 * rayDirection;

    //Discarding points outside cylinder
    float outsideTop = dot(newPt1 - endPt, -H);
    float outsideTop2;
    float outsideBottom2;
    vec3 newPt;
    newPt = newPt1;

    if(outsideTop < 0.0)
    {
        float t2 = (-b + sqrt(d))/(2*a);
        vec3 newPt2 = rayOrigin + t2 * rayDirection;

        outsideTop2 = dot(newPt2 - endPt, -H);
        if(outsideTop2 < 0.0)
            discard;

        outsideBottom2 = dot(newPt2 - base, H);
        if(outsideBottom2 < 0.0)
            discard;

        newPt = newPt2;
    }

    float outsideBottom = dot(newPt1 - base, H);
    if(outsideBottom < 0.0)
    {
        float t2 = (-b + sqrt(d))/(2*a);
        vec3 newPt2 = rayOrigin + t2 * rayDirection;

        outsideBottom2 = dot(newPt2 - base, H);
        if(outsideBottom2 < 0.0)
            discard;

        outsideTop2 = dot(newPt2 - endPt, -H);
        if(outsideTop2 < 0.0)
            discard;

        newPt = newPt2;
    }

    vec3 tmpPt = newPt - origin.xyz;
    vec3 normal = normalize(tmpPt - H * dot(tmpPt, H));

    //Extracting the z component
    vec4 projectedPt = projMatrix * vec4(newPt, 1.0);
    projectedPt /= projectedPt.w;
    gl_FragDepth = projectedPt.z * 0.5 + 0.5;

    vec3 colorTmp;
    vec3 lightDir = cameraPos - newPt;
    vec3 cameraDir = lightDir;

    if(shading == 0)
        colorTmp = phong_shading(lightDir, cameraDir, normal, color.xyz);
    else if (shading == 1)
        colorTmp = toon_shading(lightDir, normal, color.xyz);

    colorOut = vec4(colorTmp, color.a);
    normalOut.xyz = normal * 0.5 + 0.5;
}
