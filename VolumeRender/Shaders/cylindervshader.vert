#version 330
uniform mat4    modelViewMatrix;
uniform mat4    modelViewProjMatrix;
uniform float   radius;
uniform vec3    startPt;
uniform vec3    axis;
uniform vec4    color;

// Those coordinates are between 0,1
//in vec4         vertex;
in vec3         localCoordIn;

out vec3        U;
out vec3        H;
out vec3        V;
out float       cylinderLength;
out vec4        vertexBase;
out vec3        localCoord;

void main()
{
    cylinderLength = length(axis);

    //We compute the bounding box
    //We receive 8 points, and we should place this 8 points
    //at their bounding box position
    vertexBase = vec4(startPt, 1.0);

    //We receive from the program the origin that is the cylinder start
    //point. To this guy we have to add the local coordinates.
    //Local vectors, u, v, h
    vec3 h = normalize(axis);
    vec3 u = cross(h, vec3(1.0, 0.0, 0.0));

    if (length(u) < 0.001)
        u = cross(h, vec3(0.0, 0.0, 1.0));

    u = normalize(u);
    vec3 v = normalize(cross(u, h));

    //We do the addition in object space
    vec4 newVertex = vertexBase;
    newVertex.xyz += u * (localCoordIn.x*2.0 - 1.0) * radius;
    newVertex.xyz += v * (localCoordIn.y*2.0 - 1.0) * radius;
    newVertex.xyz += h * localCoordIn.z * cylinderLength;

    //Base vectors of cylinder in view space
    mat3 modelViewMat = mat3(modelViewMatrix);
    U = normalize(modelViewMat * u);
    V = normalize(modelViewMat * v);
    H = normalize(modelViewMat * h);

    //To reconstruct the current fragment position, I pass the local coordinates
    localCoord = localCoordIn;

    //Projecting
    gl_Position = modelViewProjMatrix * newVertex;
}
