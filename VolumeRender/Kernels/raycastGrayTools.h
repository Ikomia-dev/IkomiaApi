/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <string>

std::string raycastGrayTools = R"(
/////////////////////////////////////////////////////////////////////////
// Gradient functions                                                  //
/////////////////////////////////////////////////////////////////////////
__constant sampler_t samplerNearest = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
__constant sampler_t samplerLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
//#pragma OPENCL EXTENSION cl_amd_printf : enable \n

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
	  float wndWidth;
	  float wndHeight;
	  float isoValue[4];
	  float padding[2];
}rayParam;

float4 calcLighting(float4 position, float4 normal, float4 diffuseColor, float4 lightPos)
{
	float4 lightPosition[1]; 
	lightPosition[0] = lightPos;
	float4 lightColor[1];
	lightColor[0] = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
	float4 lightFactors[1];
	lightFactors[0] = (float4)(0.0f, 16.0f, 0.6f, 0.0f);
	
	float4 ambientLight = (float4)(0.2f, 0.2f, 0.2f, 1.0f);
	float4 returnval = ambientLight * diffuseColor;
	int LIGHT_COUNT = 1;
	
	for (int lightID = 0; lightID < LIGHT_COUNT; ++lightID)
	{
		
	  float4 lightVector = lightPosition[lightID] - position;
	  float lightDistance = length(lightVector);
	  float4 lightDirection = lightVector * (1.0f / lightDistance);
	 
	  //if the normal has a zero length, illuminate it as though it was
	  //not lit
	  float normal_length = length(normal);
	  normal = (normal_length == 0) ? -lightDirection : normal / normal_length;
	  
	  float lightNormDot = dot(normal, lightDirection);
	  
	  /////////////////////////////
	  //Blinn Phong lighting calculation
	  /////////////////////////////
	  
	  float4 ReflectedRay = -lightDirection - 2.0f * dot(normal, -lightDirection) * normal;//reflect(-lightDirection, normal);
	  float4 eyeDirection = normalize(-position);
	  //Specular
	  float specular = lightFactors[lightID].z * (float)(lightNormDot > 0.0f) * pow(fmax(dot(ReflectedRay, eyeDirection), 0.0f), lightFactors[lightID].y);
	  
	  float diffuse = clamp(lightNormDot, 0.0f, 1.0f);
	  
	  //Light attenuation
	  float decay_factor = 1.0f;// / (lightDistance * lightDistance);
	  
	  returnval += decay_factor * lightColor[lightID] * (specular + diffuse * diffuseColor);
	  returnval.w = diffuseColor.w;
	  
	}

  return returnval;
}

float4 calcGradientX(float4 pos, __read_only image3d_t volumeTex) {

	float4 offset = (float4)(1.f) / convert_float4(get_image_dim(volumeTex));
	float4 gradient;

	float v0  = read_imagef(volumeTex, samplerLinear, pos - (float4)(offset.x, 0.0, 0.0, 0.0)).x;
	float v1  = read_imagef(volumeTex, samplerLinear, pos - (float4)(0.0, offset.y, 0.0, 0.0)).x;
	float v2  = read_imagef(volumeTex, samplerLinear, pos - (float4)(0.0, 0.0, offset.z, 0.0)).x;
	float v3  = read_imagef(volumeTex, samplerLinear, pos + (float4)(offset.x, 0.0, 0.0, 0.0)).x;
	float v4  = read_imagef(volumeTex, samplerLinear, pos + (float4)(0.0, offset.y, 0.0, 0.0)).x;
	float v5  = read_imagef(volumeTex, samplerLinear, pos + (float4)(0.0, 0.0, offset.z, 0.0)).x;
	gradient.x = (v0 - v3)/(2.0f*offset.x);
	gradient.y = (v1 - v4)/(2.0f*offset.y);
	gradient.z = (v2 - v5)/(2.0f*offset.z);
	gradient.w = 0.0f;

	return gradient;
}

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

float4 addBlinnPhongShading(float4 color, float4 currentPosition, float4 lightPosition, float4 eyeDir, float4 normal)
{
	float4 N;
	float4 ambient;
	float4 specular;
	float4 diffuse;
	float4 L;
	float4 V;
	float4 H;
	float lightDistance;
	float HdotN;
	float attenuation;
	float normalLength;
	// Add a Blinn-Phong shading.
	N = normal;

	// Ambient lighting
	ambient = color * (float4)(0.0f, 0.0f, 0.0f, 0.0f);
	specular = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
	int nbLight = 1;
	for(int i=0; i<nbLight; ++i)
	{
		L = lightPosition- currentPosition;
		lightDistance = length(L);
		L = fast_normalize(L);

		float lambertian = fabs(dot(L, N));
		
		if(lambertian>0)
		{
			// Diffuse lighting
			diffuse += color * lambertian;

			// Specular
			V = fast_normalize(eyeDir);
			H = fast_normalize(V+L);
			HdotN = fabs(dot(H, N));
			HdotN *= HdotN; // x^2
			HdotN *= HdotN; // x^4
			HdotN *= HdotN; // x^8
			HdotN *= HdotN; // x^16
			specular += (float4)(0.6f,0.6f,0.6f,0.0f)*HdotN;
		}
	}
	//Light attenuation
	attenuation = 1.0f;// / (lightDistance * lightDistance);
	color.xyz = ambient.xyz + attenuation * (diffuse.xyz/nbLight + specular.xyz/nbLight);

	return color;
;
}

/////////////////////////////////////////////////////////////////////////
// Box intersection with ray                                           //
/////////////////////////////////////////////////////////////////////////
int intersectBox(float4 r_o, float4 r_d, float4 boxmin, float4 boxmax, float *tnear, float *tfar)
{
	// compute intersection of ray with all six bbox planes
	float4 invR = (float4)(1.0f,1.0f,1.0f,1.0f) / r_d;
	float4 tbot = invR * (boxmin - r_o);
	float4 ttop = invR * (boxmax - r_o);

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
	return ((uint)(rgba.w*255.0f)<<24) | ((uint)(rgba.z*255.0f)<<16) | ((uint)(rgba.y*255.0f)<<8) | (uint)(rgba.x*255.0f);
}
)";
