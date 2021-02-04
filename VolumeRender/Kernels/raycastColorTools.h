#ifndef RAYCASTCOLORTOOLS_H
#define RAYCASTCOLORTOOLS_H

#include <string>

std::string raycastColorTools = R"(
//#pragma OPENCL EXTENSION cl_amd_printf : enable \n
/////////////////////////////////////////////////////////////////////////
// Gradient functions                                                  //
/////////////////////////////////////////////////////////////////////////

const sampler_t samplerLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP| CLK_FILTER_LINEAR;

typedef struct{
    float density;
    float brightness;
    float transferOffset;
    float transferScale;
    float chR[2];
    float chG[2];
    float chB[2];
    float clipPlaneCenter[6];
    float K;
    float ratioX;
    float ratioY;
    float focalLength;
    float  wndWidth;
    float  wndHeight;
    float isoValue[4];
    float padding[2];
}rayParam;

typedef struct{
    float4 gradX;
    float4 gradY;
    float4 gradZ;
    float4 padding;
}colorGradient;

float4 calcGradientW(float4 pos, __read_only image3d_t volumeTex) {

    float4 offset = (float4)(1.f) / convert_float4(get_image_dim(volumeTex));
    float4 gradient;

    float v0  = read_imagef(volumeTex, samplerLinear, pos - (float4)(offset.x, 0.0, 0.0, 0.0)).w;
    float v1  = read_imagef(volumeTex, samplerLinear, pos - (float4)(0.0, offset.y, 0.0, 0.0)).w;
    float v2  = read_imagef(volumeTex, samplerLinear, pos - (float4)(0.0, 0.0, offset.z, 0.0)).w;
    float v3  = read_imagef(volumeTex, samplerLinear, pos + (float4)(offset.x, 0.0, 0.0, 0.0)).w;
    float v4  = read_imagef(volumeTex, samplerLinear, pos + (float4)(0.0, offset.y, 0.0, 0.0)).w;
    float v5  = read_imagef(volumeTex, samplerLinear, pos + (float4)(0.0, 0.0, offset.z, 0.0)).w;
    gradient.x = (v0 - v3)/(2.0f*offset.x);
    gradient.y = (v1 - v4)/(2.0f*offset.y);
    gradient.z = (v2 - v5)/(2.0f*offset.z);
    gradient.w = 0.0f;

    return gradient;
}

colorGradient calcGradientColor(float4 sample, image3d_t volumeTex) {

    float4 offset = (float4)(1.0f) / convert_float4(get_image_dim(volumeTex));
    colorGradient gradient;

    float4 v0  = read_imagef(volumeTex, samplerLinear, sample - (float4)(offset.x, 0.0, 0.0, 0.0));
    float4 v1  = read_imagef(volumeTex, samplerLinear, sample - (float4)(0.0, offset.y, 0.0, 0.0));
    float4 v2  = read_imagef(volumeTex, samplerLinear, sample - (float4)(0.0, 0.0, offset.z, 0.0));
    float4 v3  = read_imagef(volumeTex, samplerLinear, sample + (float4)(offset.x, 0.0, 0.0, 0.0));
    float4 v4  = read_imagef(volumeTex, samplerLinear, sample + (float4)(0.0, offset.y, 0.0, 0.0));
    float4 v5  = read_imagef(volumeTex, samplerLinear, sample + (float4)(0.0, 0.0, offset.z, 0.0));

    v0 = (v0 - v3);///(2*offset.x);
    v1 = (v1 - v4);///(2*offset.y);
    v2 = (v2 - v5);///(2*offset.z);

    gradient.gradX.x = v0.x;
    gradient.gradX.y = v1.x;
    gradient.gradX.z = v2.x;
    gradient.gradX.w = 0.0;

    gradient.gradY.x = v0.y;
    gradient.gradY.y = v1.y;
    gradient.gradY.z = v2.y;
    gradient.gradY.w = 0.0;

    gradient.gradZ.x = v0.z;
    gradient.gradZ.y = v1.z;
    gradient.gradZ.z = v2.z;
    gradient.gradZ.w = 0.0;

    return gradient;
}

float4 addBlinnPhongShading(float4 color, image3d_t volume, float4 currentPosition, float4 lightPosition[], float4 eyePosition)
{
    // Add a Blinn-Phong shading.
    colorGradient colorGrad = calcGradientColor(currentPosition, volume);
    float4 Gb = colorGrad.gradX;
    float4 Gg = colorGrad.gradY;
    float4 Gr = colorGrad.gradZ;
    float4 Gcolor = fast_normalize(Gr + Gb+ Gg);

    // Ambient lighting
    float4 ambient = color * (float4)(0.0f,0.0f,0.0f,0.0f);
    float4 specular = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    int nbLight = 1;
    for(int i=0; i<nbLight; ++i)
    {
        float4 L = fast_normalize(lightPosition[i]-currentPosition);
        float4 V = fast_normalize(eyePosition-currentPosition);
        float4 H = fast_normalize(V+L);

        Gr = fast_normalize(Gr);
        Gg = fast_normalize(Gg);
        Gb = fast_normalize(Gb);

        // Diffuse lighting
        diffuse.x += color.x * fabs(dot(Gcolor, L));
        diffuse.y += color.y * fabs(dot(Gcolor, L));
        diffuse.z += color.z * fabs(dot(Gcolor, L));
        // Specular
        float HdotN = fabs(dot(H, Gcolor));
            HdotN *= HdotN;
            HdotN *= HdotN;
            HdotN *= HdotN;
            HdotN *= HdotN;
        specular = specular + (float4)(0.6f,0.6f,0.6f,0.0f)*HdotN;//pow(fmax(dot(H, Gcolor), 0.0), 16.0f);
    }
    color.xyz = ambient.xyz + diffuse.xyz/nbLight + specular.xyz/nbLight;

    return color;
}

float4 addBlinnPhongShadingX(float4 color, image3d_t volume, float4 currentPosition, float4 lightPosition[], float4 eyePosition)
{
    // Add a Blinn-Phong shading.
    float4 normal = calcGradientW(currentPosition, volume);
    normal = fast_normalize(normal);
    if(fast_length(normal) == 0.0f)
        return color;
    // Ambient lighting
    float4 ambient = color * (float4)(0.0f,0.0f,0.0f,0.0f);
    float4 specular = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    int nbLight = 1;
    for(int i=0; i<nbLight; ++i)
    {
        float4 L = fast_normalize(lightPosition[i]-currentPosition);
        float4 V = fast_normalize(eyePosition-currentPosition);
        float4 H = fast_normalize(V+L);

        // Diffuse lighting
        diffuse.x += color.x * fabs(dot(normal, L));
        diffuse.y += color.y * fabs(dot(normal, L));
        diffuse.z += color.z * fabs(dot(normal, L));
        // Specular
        float HdotN = fabs(dot(H, normal));
            HdotN *= HdotN;
            HdotN *= HdotN;
            HdotN *= HdotN;
            HdotN *= HdotN;
        specular = specular + (float4)(0.6f,0.6f,0.6f,0.0f)*HdotN;
    }
    color.xyz = ambient.xyz + diffuse.xyz/nbLight + specular.xyz/nbLight;

    return color;
}
/////////////////////////////////////////////////////////////////////////
// Box intersection with ray                                           //
/////////////////////////////////////////////////////////////////////////

int intersectBox(float4 eyeRay_o, float4 eyeRay_d, float4 boxmin, float4 boxmax, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox planes
    float4 invR = (float4)(1.0f,1.0f,1.0f,1.0f) / eyeRay_d;
    float4 tbot = invR * (boxmin - eyeRay_o);
    float4 ttop = invR * (boxmax - eyeRay_o);

    // re-order intersections to find smallest and largest on each axis
    float4 tmin = min(ttop, tbot);
    float4 tmax = max(ttop, tbot);

    // find the largest tmin and the smallest tmax
    float largest_tmin = max(max(tmin.x, tmin.y), max(tmin.x, tmin.z));
    float smallest_tmax = min(min(tmax.x, tmax.y), min(tmax.x, tmax.z));

    *tnear = largest_tmin;
    *tfar = smallest_tmax;

    return smallest_tmax > largest_tmin;
}

/////////////////////////////////////////////////////////////////////////
// Conversion float to int                                             //
/////////////////////////////////////////////////////////////////////////
uint rgbaFloatToInt(float4 rgba)
{
    rgba.x = clamp(rgba.x,0.0f,1.0f);
    rgba.y = clamp(rgba.y,0.0f,1.0f);
    rgba.z = clamp(rgba.z,0.0f,1.0f);
    rgba.w = clamp(rgba.w,0.0f,1.0f);
    return ((uint)(rgba.w*255.0f)<<24) | ((uint)(rgba.x*255.0f)<<16) | ((uint)(rgba.y*255.0f)<<8) | (uint)(rgba.z*255.0f);
}
)";
#endif // RAYCASTCOLORTOOLS_H
