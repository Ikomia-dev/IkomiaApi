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

#include "raycastGrayTools.h"
#include "raycastGrayF2B.h"
#include "raycastGrayIso.h"
#include "raycastGrayMIP.h"
#include "raycastGrayMinIP.h"

#include <string>

std::string kernelRaycastGray = raycastGrayTools 
+ raycastGrayF2B
+ raycastGrayIso 
+ raycastGrayMIP 
+ raycastGrayMinIP;

/*
/////////////////////////////////////////////////////////////////////////
// Front to Back Raycasting rendering for grayscale image              //
/////////////////////////////////////////////////////////////////////////
__kernel void renderGray_F2B_PreIntegrated(
		__global uint *d_output, 
		rayParam myParam,
		__constant float* invViewMatrix,
		__read_only image3d_t volume,
		__read_only image2d_t transferFunc,
		  sampler_t volumeSampler,
		  sampler_t transferFuncSampler,
		  __global float* d_depth,
		  __constant float* modelViewProjMatrix
		 )

{	
	uint x = get_global_id(0);
	uint y = get_global_id(1);

	// Resize coordinates (x,y) from [0, wndWidth]x[0, wndHeight] in [-1, 1]x[-1, 1]
	float u =  K*x  - ratioX;
	float v =  K*y  - ratioY;
	
	// constant step inside texture along ray
	//const float tstep = 1.0f/255.0f;

	// Bounding box size

	float4 boxMin = (float4)(-alpha, -beta, -delta, 1.0f);
	float4 boxMax = (float4)( alpha,  beta,  delta, 1.0f);
	// calculate eye ray in world space
	float4 eyeRay_o;
	float4 eyeRay_d;

	// A optimiser par compilo
	//float focalLength = fmin(ratioX, ratioY) * 0.5 * 2.0 / native_tan(3.141592653589793f/8.0); //= 1/2 * borderLength / tan(FOV/2)
	// eyeRay_o = Camera Position (Origin)
   
	eyeRay_o = (float4)(invViewMatrix[3], invViewMatrix[7], invViewMatrix[11], 1.0f);   
	// eyeRay_d = Current direction (Destination)
	float4 rayDir = fast_normalize(((float4)(u, v, -FOCALLENGTH, 0.0f)));
	eyeRay_d.x = dot(rayDir, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2], invViewMatrix[3])));
	eyeRay_d.y = dot(rayDir, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6], invViewMatrix[7])));
	eyeRay_d.z = dot(rayDir, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
	eyeRay_d.w = 0.0f;

	float tnear;
	float tfar;
	int hit = intersectBox(eyeRay_o, eyeRay_d, boxMin, boxMax, &tnear, &tfar);
	if (!hit) 
	{
		if ((x < myParam->wndWidth) && (y < myParam->wndHeight)) 
		{
			// write output color
			uint i = x + (y * myParam->wndWidth);
			d_output[i] = 0;
			d_depth[i] = 1;
		}
		return;
	}

	if (tnear < 0.0f) 
		tnear = 0.0f;     // clamp to near plane

	// *****************************************************************
	// march along ray from back to front, accumulating color
	// *****************************************************************
	float4 temp = (float4)(0.0f,0.0f,0.0f,0.0f);
	float4 lightPosition[1];
	lightPosition[0] = eyeRay_o;
	lightPosition[0].x = native_divide(lightPosition[0].x, alpha);
	lightPosition[0].y = -native_divide(lightPosition[0].y, beta);
	lightPosition[0].z = -native_divide(lightPosition[0].z, delta);
	lightPosition[0] = lightPosition[0] * 0.5f + 0.5f;

	float4 eyePosition = lightPosition[0];

	// init ray limits
	float4 rayStart = eyeRay_o + eyeRay_d*tnear;
	float4 rayStop = eyeRay_o + eyeRay_d*tfar;

	// Get depth value from position in projetive space
	float4 depthVec;
	depthVec.x = dot(rayStop, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
	depthVec.y = dot(rayStop, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
	depthVec.z = dot(rayStop, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
	depthVec.w = dot(rayStop, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));

	// transform from object space to coordinate space
	// map x from [-alpha,alpha] to [-1,1]
	rayStart.x = native_divide(rayStart.x, alpha);
	rayStop.x  = native_divide(rayStop.x, alpha);
	// map y from [-beta,beta] to [-1,1]
	rayStart.y = -native_divide(rayStart.y, beta);
	rayStop.y  = -native_divide(rayStop.y, beta);
	// map z from [-delta,delta] to [-1,1]
	rayStart.z = -native_divide(rayStart.z, delta);
	rayStop.z  = -native_divide(rayStop.z, delta);
	// map position from [-1,1]x[-1,1]x[-1,1] to [0,1]x[0,1]x[0,1] coordinates
	rayStart = rayStart * 0.5f + 0.5f;
	rayStop = rayStop * 0.5f + 0.5f;

	// Init first position
	float4 pos = rayStart;
	// Init ray step
	float4 step = fast_normalize(rayStop - rayStart) * TSTEP;
	float travel = fast_distance(rayStop, rayStart);
	float4 sampleInit = read_imagef(volume, volumeSampler, pos);
	bool bFirstElt = true;
	for(uint i=0; i<2000 && travel>0.0; i++, pos+=step, travel-=TSTEP) // nb iteration depends on stepsize, should be large enough
	{		 
		// read from 3D texture        
		// return bgra values in [0,1]^4 due to CL_RGBA and CL_UNORM_INT8
		 
		float4 sampleEnd = read_imagef(volume, volumeSampler, pos+step); 
		float4 color = (float4)(0.0f,0.0f,0.0f,0.0f);
		if( !(sampleInit.x>=myParam->chR[0] && sampleInit.x<=myParam->chR[1]) )
		{
			sampleInit = color;
			sampleEnd = color;
		}

		// *****************************************************************
		// lookup in transfer function texture
		// *****************************************************************
		float2 transfer_pos = (float2)((sampleInit.x-myParam->transferOffset)*myParam->transferScale, (sampleEnd.x-myParam->transferOffset)*myParam->transferScale);
		color = read_imagef(transferFunc, transferFuncSampler, transfer_pos);

		// Add a Blinn-Phong shading.  

		//color = addBlinnPhongShading(color, volume, pos, lightPosition, eyePosition);	
		color.w *= myParam->density;

		// *****************************************************************
		// accumulate result
		// *****************************************************************
		//calculate ray integral
		temp.xyz = temp.xyz + (1.0 - temp.w)  * color.w * color.xyz;
		temp.w = temp.w + (1.0 - temp.w) * color.w;
		
		if(temp.w != 0 && bFirstElt)
		{
			float4 newPos = pos*2.0f - 1.0f;
			newPos.x *= alpha;
			newPos.y *= -beta;
			newPos.z *= -delta;
			depthVec.x = dot(newPos, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
			depthVec.y = dot(newPos, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
			depthVec.z = dot(newPos, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
			depthVec.w = dot(newPos, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));
			bFirstElt = false;
		}

		// early ray termination
		if(temp.w > 0.99f)
			break;

		sampleInit = sampleEnd;
		
	}
	temp *= myParam->brightness;
		
	if ((x < myParam->wndWidth) && (y < myParam->wndHeight)) {
		// write output color
		uint i = x + (y * myParam->wndWidth);
		d_output[i] = rgbaFloatToInt(temp);
		d_depth[i] = (depthVec.z/depthVec.w + 1.0f) * 0.5f;
	}
}

/////////////////////////////////////////////////////////////////////////
// Front to Back Raycasting Isosurface rendering              //
/////////////////////////////////////////////////////////////////////////
__kernel void renderIso_F2B_PreIntegrated(
		__global uint *d_output, 
		rayParam myParam,
		__constant float* invViewMatrix,
		__read_only image3d_t volume,
		__read_only image2d_t transferFunc,
		  sampler_t volumeSampler,
		  sampler_t transferFuncSampler,
		  __global float* d_depth,
		  __constant float* modelViewProjMatrix
		 )

{	
	uint x = get_global_id(0);
	uint y = get_global_id(1);

	// Resize coordinates (x,y) in [-1, 1]x[-1, 1]
	float u =  K*x  - ratioX;
	float v =  K*y  - ratioY;

	// constant step inside texture along ray
	//const float tstep = 1.0f/255.0f;

	// Bounding box limit
	float4 boxMin = (float4)(-alpha, -beta, -delta, 1.0f);
	float4 boxMax = (float4)( alpha,  beta,  delta, 1.0f);

	// calculate eye ray in world space
	float4 eyeRay_o;
	float4 eyeRay_d;

	// A optimiser par compilo
	//float focalLength = fmin(ratioX, ratioY) * 0.5 * 2.0 / tan(3.141592653589793f/8.0); //= 1/2 * borderLength / tan(FOV/2)
	
	// eyeRay_o = Camera Position (Origin) in object space
	eyeRay_o = (float4)(invViewMatrix[3], invViewMatrix[7], invViewMatrix[11], 1.0f);   

	// camEyeRay_d = ray direction in camera space
	float4 rayDir = normalize(((float4)(u, v, -FOCALLENGTH, 0.0f)));
	
	// eyeRay_d = ray direction in object space
	eyeRay_d.x = dot(rayDir, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
	eyeRay_d.y = dot(rayDir, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
	eyeRay_d.z = dot(rayDir, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
	eyeRay_d.w = 0.0f;

	float tnear;
	float tfar;
	int hit = intersectBox(eyeRay_o, eyeRay_d, boxMin, boxMax, &tnear, &tfar);
	if (!hit) 
	{
		if ((x < myParam->wndWidth) && (y < myParam->wndHeight)) 
		{
			// write output color
			uint i =(y * myParam->wndWidth) + x; 
			d_output[i] = 0;
			d_depth[i] = 1;
		}
		return;
	}
	if (tnear < 0.0f)
		tnear = 0.0f;     // clamp to near plane

	// *****************************************************************
	// march along ray from front to back, accumulating color
	// *****************************************************************
	float4 temp = (float4)(0.0f,0.0f,0.0f,0.0f);
	float4 color = (float4)(0.0f,0.0f,0.0f,0.0f);
	// Trouver valeur de lumière invariant dans object space...
	float4 lightPosition[1];
	lightPosition[0] = eyeRay_o;
	lightPosition[0].x = native_divide(lightPosition[0].x, alpha);
	lightPosition[0].y = -native_divide(lightPosition[0].y, beta);
	lightPosition[0].z = -native_divide(lightPosition[0].z, delta);
	lightPosition[0] = lightPosition[0] * 0.5f + 0.5f;

	float4 eyePosition = lightPosition[0];

	// init ray limits
	float4 rayStart = eyeRay_o + eyeRay_d*tnear;
	float4 rayStop = eyeRay_o + eyeRay_d*tfar;

	// Get depth value from position in projetive space
	float4 depthVec;
	depthVec.x = dot(rayStop, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
	depthVec.y = dot(rayStop, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
	depthVec.z = dot(rayStop, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
	depthVec.w = dot(rayStop, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));

	// transform from object space to coordinate space
	// map x from [-alpha,alpha] to [-1,1]
	rayStart.x = native_divide(rayStart.x, alpha);
	rayStop.x  = native_divide(rayStop.x, alpha);
	// map y from [-beta,beta] to [-1,1]
	rayStart.y = -native_divide(rayStart.y, beta);
	rayStop.y  = -native_divide(rayStop.y, beta);
	// map z from [-delta,delta] to [-1,1]
	rayStart.z = -native_divide(rayStart.z, delta);
	rayStop.z  = -native_divide(rayStop.z, delta);
	// map position from [-1,1]x[-1,1]x[-1,1] to [0,1]x[0,1]x[0,1] coordinates
	rayStart = rayStart * 0.5f + 0.5f;
	rayStop = rayStop * 0.5f + 0.5f;

	// Init first position
	float4 pos = rayStart;
	// Init ray step
	float4 step = normalize(rayStop - rayStart) * TSTEP;
	float travel = distance(rayStop, rayStart);

	float4 sampleInit = read_imagef(volume, volumeSampler, pos); 
	bool bFirstElt = true;
	for(uint i=0; i<2000 && travel>0.0; i++, pos+=step, travel-=TSTEP) // nb iteration depends on stepsize, should be large enough
	{		
		// read from 3D texture        
		// return bgra values in [0,1]^4 due to CL_RGBA and CL_UNORM_INT8
		float4 sampleEnd = read_imagef(volume, volumeSampler, pos); 
		color = (float4)(0.0f,0.0f,0.0f,0.0f);

		if( sampleInit.x>=myParam->chR[0] && sampleInit.x<=myParam->chR[1] )
		{
			float2 transfer_pos = (float2)(sampleInit.x, sampleEnd.x);
			color = read_imagef(transferFunc, transferFuncSampler, transfer_pos);
			temp.w = fmax(color.w, temp.w);
		}

		temp.xyz = color.xyz;

		if(temp.w != 0 && bFirstElt)
		{
			float4 newPos = pos*2.0f - 1.0f;
			newPos.x *= alpha;
			newPos.y *= -beta;
			newPos.z *= -delta;
			depthVec.x = dot(newPos, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
			depthVec.y = dot(newPos, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
			depthVec.z = dot(newPos, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
			depthVec.w = dot(newPos, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));
			bFirstElt = false;
		}

		if(temp.w > 0.99)
		{
			// Add a Blinn-Phong shading.  
			//temp = addBlinnPhongShading(temp, volume, pos, lightPosition, eyePosition);
			break;
		}
		sampleInit = sampleEnd;
	}
		
	if ((x < myParam->wndWidth) && (y < myParam->wndHeight)) {
		// write output color
		uint i =(y * myParam->wndWidth) + x;
		d_output[i] = rgbaFloatToInt(temp);
		d_depth[i] = (depthVec.z/depthVec.w + 1.0f) * 0.5f;
	}
}*/
